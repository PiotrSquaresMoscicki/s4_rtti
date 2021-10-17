#pragma once

#include "rtti_fwd.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class Object {
    public:
        friend Type;

        template <typename TYPE>
        void Object(TYPE* obj);
        void Object(void* obj, const Type* type);

        const Type* type() const;
        const void* data() const:

        template <typename TYPE>
        void set(TYPE* obj);
        void set(void* obj, const Type* type);

        void delete_object() &&;
        Buffer call_destructor() &&;
        void copy(const Object& src);
        void move(Object&& src);

    private:
        const Type* m_type = nullptr;
        void* m_data = nullptr;

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