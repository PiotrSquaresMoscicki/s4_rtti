#pragma once

#include <string>
#include <memory>

#include "rtti_fwd.hpp"
#include "attributes.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class Type {
    public:
        Type(std::string name, size_t size, Attributes attributes);

        const std::string& name() const { return m_name; }
        size_t size() const { return m_size; }
        const Attributes& attributes() const { return m_attributes; }
        template <typename ATTRIBUTE> const ATTRIBUTE* attribute() const;

        virtual Object new_object() const = 0;
        virtual Object call_constructor(Buffer&& buff) const = 0;
        virtual void delete_object(Object&& obj) const = 0;
        virtual Buffer call_destructor(Object&& obj) const = 0;
        virtual void copy(ObjectRef& dst, const ObjectRef& src) const = 0;
        virtual Object copy_construct(const ObjectRef& src) const = 0;
        virtual void move(ObjectRef& dst, ObjectRef& src) const = 0;
        virtual Object move_construct(ObjectRef& src) const = 0;

        virtual const Fundamental* as_fundamental() const = 0;
        virtual const Enum* as_enum() const = 0;
        virtual const Class* as_class() const = 0;

    protected:
        void* writable_data(Buffer& buff) const;
        void* writable_data(ObjectRef& obj) const;
        void* move_data(Buffer&& buff) const;
        void* move_data(ObjectRef&& obj) const;

    private:
        std::string m_name;
        size_t m_size = 0;
        Attributes m_attributes;

    }; // class Type

} // namespace rtti