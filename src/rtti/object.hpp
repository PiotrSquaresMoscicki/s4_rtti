#pragma once

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
        ObjectRef(TYPE* obj);
        ObjectRef(void* obj, const Type* type);

        const Type* type() const;
        const void* value() const;

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