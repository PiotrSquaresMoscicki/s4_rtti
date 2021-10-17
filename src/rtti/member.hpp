#pragma once

#include <string>

#include "rtti_fwd.hpp"

namespace rtti {

    class Member {
    public:
        Member(std::string name, const Type* type, const Class* declaring_class)

        const std::string& name() const { return m_name; }
        const Type* type() const { return m_type; }
        const Class* declaring_class() const { return m_declaring_class; }

        virtual const ObjectRef value(const ObjectRef& object) const = 0;
        virtual void copy_assign(ObjectRef& object, const ObjectRef& src) = 0;
        virtual void move_assign(Objectref& object, ObjectRef& src) = 0

    private:
        const std::string name;
        const Type* const m_type = nullptr;
        const Class* const m_declaring_class = nullptr;

    }; // class member

} // namespace rtti