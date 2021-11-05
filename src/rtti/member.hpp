#pragma once

#include <string>

#include "rtti_fwd.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Member {
    public:
        Member(std::string name, const Type* type, const Class* declaring_class
            , Attributes attributes)
            : m_name(std::move(name)), m_type(type), m_declaring_class(declaring_class)
            , m_attributes(std::move(attributes)) {}

        const std::string& name() const { return m_name; }
        const Type* type() const { return m_type; }
        const Class* declaring_class() const { return m_declaring_class; }
        const Attributes& attributes() const { return m_attributes; }
        template <typename ATTRIBUTE> const ATTRIBUTE* attribute() const;

        virtual const ObjectRef value(const ObjectRef& object) const = 0;
        virtual void copy_assign(ObjectRef& object, const ObjectRef& src) const = 0;
        virtual void move_assign(ObjectRef& object, ObjectRef& src) const = 0;

        virtual const Field* as_field() const = 0;
        virtual const Property* as_property() const = 0;

    private:
        const std::string m_name;
        const Type* const m_type = nullptr;
        const Class* const m_declaring_class = nullptr;
        const Attributes m_attributes;

    }; // class member

} // namespace rtti