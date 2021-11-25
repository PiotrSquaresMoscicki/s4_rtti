#pragma once

#include <cassert>

#include <core/util/res.hpp>

#include "rtti_fwd.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT ObjectRef {
    public:
        enum class ErrValue {
            INVALID_OBJECT
        };

        enum class ErrType {
            INVALID_OBJECT
        };

        template <typename FUNDAMENTAL> friend class FundamentalInstance;
        template <typename ENUM> friend class EnumInstance;
        template <typename CLASS> friend class ClassInstance;

        ObjectRef() = default;
        template <typename TYPE>
        ObjectRef(TYPE* obj) : ObjectRef(reinterpret_cast<void*>(obj), static_type<TYPE>()) {}
        ObjectRef(void* obj, const Type* type);
        virtual ~ObjectRef() = default;

        bool is_valid() const { return m_value && m_type; }
        Res<void*, ErrValue> value();
        Res<const void*, ErrValue> value() const;
        Res<void*, ErrValue> steal_value() &&;
        Res<const Type*, ErrType> type() const;

        template <typename TYPE>
        void copy_assign(const TYPE& value) { copy_assign(ObjectRef(&value)); }
        void copy_assign(const ObjectRef& src);

        template <typename TYPE>
        void move_assign(TYPE&& value) { move_assign(ObjectRef(&value)); }
        void move_assign(ObjectRef& src);

    protected:
        void* m_value = nullptr;
        const Type* m_type = nullptr;

    }; // class ObjectRef

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Object : public ObjectRef {
    public:
        template <typename FUNDAMENTAL> friend class FundamentalInstance;
        template <typename ENUM> friend class EnumInstance;
        template <typename CLASS> friend class ClassInstance;

        Object() = default;
        template <typename TYPE>
        Object(TYPE* obj) : ObjectRef(reinterpret_cast<void*>(obj), static_type<TYPE>()) {}
        Object(void* obj, const Type* type);
        ~Object();

        template <typename TYPE>
        void change_pointed_object(TYPE* obj);
        void change_pointed_object(void* obj, const Type* type);

        void delete_object() &&;
        BufferRef call_destructor() &&;

    }; // class Object

    //*********************************************************************************************
    template <typename TYPE>
    void Object::change_pointed_object(TYPE* obj) {
        change_pointed_object(static_cast<void*>(obj), static_type<TYPE>());
    }

} // namespace rtti