#pragma once

#include "rtti_fwd.hpp"
#include "member.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Field : public Member {
    public:
        Field(std::string name, const Type* type, const Class* declaring_class
            , Attributes attributes) 
            : Member(std::move(name), type, declaring_class, std::move(attributes)) {}
            
        const Field* as_field() const { return this; };
        const Property* as_property() const { return nullptr; }
    }; // class Field

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename CLASS, typename FIELD>
    class S4_RTTI_EXPORT FieldInstance : public Field {
    public:
        FieldInstance(std::string name, FIELD CLASS::*const field);
        FieldInstance(std::string name, FIELD CLASS::*const field, Attributes attributes);
        
        const ObjectRef value(const ObjectRef& object) const override;
        void copy_assign(ObjectRef& object, const ObjectRef& src) const override;
        void move_assign(ObjectRef& object, ObjectRef& src) const override;

    private:
        FIELD CLASS::*const m_field = nullptr;
    }; // class FieldInstance

    //*********************************************************************************************
    template <typename CLASS, typename FIELD>
    FieldInstance<CLASS, FIELD>::FieldInstance(std::string name, FIELD CLASS::*const)
        : Field(std::move(name), static_type<FIELD>(), static_class<CLASS>(), {}) 
    {}

    //*********************************************************************************************
    template <typename CLASS, typename FIELD>
    FieldInstance<CLASS, FIELD>::FieldInstance(std::string name, FIELD CLASS::*const
        , Attributes attributes)
        : Field(std::move(name), static_type<FIELD>(), static_class<CLASS>(), std::move(attributes)) 
    {}

    //*********************************************************************************************
    template <typename CLASS, typename FIELD>
    const ObjectRef FieldInstance<CLASS, FIELD>::value(const ObjectRef&) const {
        return Object(new CLASS());
    }

    //*********************************************************************************************
    template <typename CLASS, typename FIELD>
    void FieldInstance<CLASS, FIELD>::copy_assign(ObjectRef&, const ObjectRef&) const {
    }

    //*********************************************************************************************
    template <typename CLASS, typename FIELD>
    void FieldInstance<CLASS, FIELD>::move_assign(ObjectRef&, ObjectRef&) const {
    }

} // namespace rtti