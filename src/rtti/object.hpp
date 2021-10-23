#pragma once

#include <cassert>

#include "rtti_fwd.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class ObjectRef {
    public:
        friend Type;

        ObjectRef() = default;
        template <typename TYPE>
        ObjectRef(TYPE* obj) : ObjectRef(reinterpret_cast<void*>(obj), static_type<TYPE>()) {}
        ObjectRef(void* obj, const Type* type);

        bool is_valid() const { return m_value && m_type; }
        const void* value() const { assert(is_valid()); return m_value; }
        const Type* type() const { assert(is_valid()); return m_type; }

        void copy(const ObjectRef& src);
        void move(ObjectRef&& src);

    protected:
        void* m_value = nullptr;
        const Type* const m_type = nullptr;

    }; // class ObjectRef

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class Object : public ObjectRef {
    public:
        friend Type;

        Object() = default;
        template <typename TYPE>
        Object(TYPE* obj);
        Object(void* obj, const Type* type);

        template <typename TYPE>
        void change_pointed_object(TYPE* obj);
        void change_pointed_object(void* obj, const Type* type);

        void delete_object() &&;
        Buffer call_destructor() &&;

    }; // class Object

    //*********************************************************************************************
    template <typename TYPE>
    Object::Object(TYPE* obj) : Object(static_cast<void*>(obj), static_type<TYPE>()) {}

    //*********************************************************************************************
    template <typename TYPE>
    void Object::change_pointed_object(TYPE* obj) {
        change_pointed_object(static_cast<void*>(obj), static_type<TYPE>());
    }

} // namespace rtti