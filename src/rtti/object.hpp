#pragma once

#include <cassert>

#include "rtti_fwd.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class ObjectRef {
    public:
        template <typename FUNDAMENTAL> friend class FundamentalInstance;
        template <typename ENUM> friend class EnumInstance;
        template <typename CLASS> friend class ClassInstance;

        ObjectRef() = default;
        template <typename TYPE>
        ObjectRef(TYPE* obj) : ObjectRef(reinterpret_cast<void*>(obj), static_type<TYPE>()) {}
        ObjectRef(void* obj, const Type* type);

        bool is_valid() const { return m_value && m_type; }
        const void* value() const { assert(is_valid()); return m_value; }
        const Type* type() const { assert(is_valid()); return m_type; }

        template <typename TYPE>
        void copy_assign(const TYPE& value) { copy_assign(ObjectRef(&value)); }
        void copy_assign(const ObjectRef& src);

        template <typename TYPE>
        void move_assign(TYPE&& value) { move_assign(ObjectRef(&value)); }
        void move_assign(ObjectRef&& src);

    protected:
        void* m_value = nullptr;
        const Type* m_type = nullptr;

    }; // class ObjectRef

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class Object : public ObjectRef {
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
        Buffer call_destructor() &&;

    }; // class Object

    //*********************************************************************************************
    template <typename TYPE>
    void Object::change_pointed_object(TYPE* obj) {
        change_pointed_object(static_cast<void*>(obj), static_type<TYPE>());
    }

} // namespace rtti