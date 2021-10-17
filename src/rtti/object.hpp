#pragma once

#include "rtti_fwd.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class ObjectRef {
    public:
        friend Type;

        template <typename TYPE>
        void ObjectRef(TYPE* obj);
        void ObjectRef(void* obj, const Type* type);

        const Type* type() const;
        const void* value() const:

        void copy(const Object& src);
        void move(Object&& src);

    private:
        const Type* const m_type = nullptr;
        void* m_data = nullptr;

    }; // class ObjectRef

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class Object : public ObjectRef {
    public:
        friend Type;

        template <typename TYPE>
        void Object(TYPE* obj);
        void Object(void* obj, const Type* type);

        template <typename TYPE>
        void change_pointed_object(TYPE* obj);
        void change_pointed_object(void* obj, const Type* type);

        void delete_object() &&;
        Buffer call_destructor() &&;

    }; // class Object

    //*********************************************************************************************
    template <typename TYPE>
    void Object::Object(TYPE* obj) : Object(static_cast<void*>(obj), get_type<TYPE>()) {}

    //*********************************************************************************************
    template <typename TYPE>
    void Object::set(TYPE* obj) {
        set(static_cast<void*>(obj), get_type<TYPE>());
    }

} // namespace rtti