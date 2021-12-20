#pragma once

#include "rtti_fwd.hpp"
#include "member.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Field : public Member {
    public:
        Field(std::string name, TypePtr type, ClassPtr declaring_class
            , Meta meta) 
            : Member(std::move(name), type, declaring_class, std::move(meta)) {}
            
        const Field* as_field() const { return this; };
        const Property* as_property() const { return nullptr; }
    }; // class Field

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS, typename FIELD>
    class FieldInstance : public Field {
    public:
        FieldInstance(std::string name, FIELD CLASS::*const field);
        FieldInstance(std::string name, FIELD CLASS::*const field, Meta meta);
        
        const ObjectRef value(const ObjectRef& object) const override;
        void copy_assign(ObjectRef& object, const ObjectRef& src) const override;
        void move_assign(ObjectRef& object, ObjectRef& src) const override;

    private:
        FIELD CLASS::*const m_field = nullptr;

    }; // class FieldInstance

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS, typename FIELD>
    FieldInstance<CLASS, DECLARING_CLASS, FIELD>::FieldInstance(std::string name, FIELD CLASS::*const)
        : Field(std::move(name), static_type<FIELD>(), static_class<CLASS>(), {}) 
    {
        const_cast<Class*>(DECLARING_CLASS::static_class())->m_members.push_back(this);
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS, typename FIELD>
    FieldInstance<CLASS, DECLARING_CLASS, FIELD>::FieldInstance(std::string name, FIELD CLASS::*const
        , Meta meta)
        : Field(std::move(name), static_type<FIELD>(), static_class<CLASS>(), std::move(meta)) 
    {}

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS, typename FIELD>
    const ObjectRef FieldInstance<CLASS, DECLARING_CLASS, FIELD>::value(const ObjectRef&) const {
        return Object(new CLASS());
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS, typename FIELD>
    void FieldInstance<CLASS, DECLARING_CLASS, FIELD>::copy_assign(ObjectRef&, const ObjectRef&) const {
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS, typename FIELD>
    void FieldInstance<CLASS, DECLARING_CLASS, FIELD>::move_assign(ObjectRef&, ObjectRef&) const {
    }

} // namespace rtti