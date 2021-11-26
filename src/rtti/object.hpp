#pragma once

#include <cassert>

#include <core/util/res.hpp>

#include "rtti_fwd.hpp"
#include "type.hpp"

namespace rtti {

    using namespace core::util;

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

        enum class ErrSize {
            INVALID_OBJECT
        };

        template <typename FUNDAMENTAL> friend class FundamentalInstance;
        template <typename ENUM> friend class EnumInstance;
        template <typename CLASS> friend class ClassInstance;

        ObjectRef() = default;
        ObjectRef(const ObjectRef& other) = delete;
        ObjectRef(ObjectRef&& other);
        ObjectRef& operator=(const ObjectRef& other) = delete;
        ObjectRef& operator=(ObjectRef&& other);

        template <typename TYPE>
        ObjectRef(TYPE* obj) : ObjectRef(reinterpret_cast<void*>(obj), static_type<TYPE>()) {}
        template <typename TYPE>
        ObjectRef(TYPE* obj, size_t size) 
            : ObjectRef(reinterpret_cast<void*>(obj), static_type<TYPE>(), size) {}
        ObjectRef(void* obj, const Type* type) : ObjectRef(obj, type, type->size()) {}
        ObjectRef(void* obj, const Type* type, size_t size);
        virtual ~ObjectRef() = default;

        bool is_valid() const { return m_value && m_type && m_size; }
        template <typename T> Res<T*, ErrValue> value_as();
        template <typename T> Res<const T*, ErrValue> value_as() const;
        Res<void*, ErrValue> value();
        Res<const void*, ErrValue> value() const;
        Res<void*, ErrValue> steal_value() &&;
        Res<const Type*, ErrType> type() const;
        Res<size_t, ErrSize> size() const;

        template <typename TYPE>
        void copy_assign(const TYPE& value) { copy_assign(ObjectRef(&value)); }
        void copy_assign(const ObjectRef& src);

        template <typename TYPE>
        void move_assign(TYPE&& value) { move_assign(ObjectRef(&value)); }
        void move_assign(ObjectRef& src);

    protected:
        void* m_value = nullptr;
        const Type* m_type = nullptr;
        size_t m_size = 0;

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
        Object(const Object& other) = delete;
        Object(Object&& other) = default;
        Object& operator=(const Object& other) = delete;
        Object& operator=(Object&& other) = default;

        using ObjectRef::ObjectRef;
        ~Object();

        template <typename TYPE>
        void change_pointed_object(TYPE* obj);
        void change_pointed_object(void* obj, const Type* type);

        void delete_object() &&;
        BufferRef call_destructor() &&;

    }; // class Object

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename T>
    Res<T*, ObjectRef::ErrValue> ObjectRef::value_as() {
        assert(static_type<T>() == m_type); 
        return Ok(reinterpret_cast<T*>(m_value));
    }

    //*********************************************************************************************
    template <typename T>
    Res<const T*, ObjectRef::ErrValue> ObjectRef::value_as() const {
        assert(static_type<T>() == m_type); 
        return Ok(reinterpret_cast<const T*>(const_cast<const void*>(m_value)));
    }

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename TYPE>
    void Object::change_pointed_object(TYPE* obj) {
        change_pointed_object(static_cast<void*>(obj), static_type<TYPE>());
    }

} // namespace rtti