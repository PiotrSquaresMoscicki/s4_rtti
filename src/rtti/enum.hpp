#pragma once

#include <vector>
#include <cassert>

#include "type_instance.hpp"
#include "object.hpp"
#include "buffer.hpp"

namespace rtti {

    class EnumValue;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Enum : public virtual Type {
    public:
        Enum(std::vector<EnumValue> values);

        const std::vector<const EnumValue*>& values() const { return m_values_ptrs; }

        bool is_fundamental() const override { return false; }
        bool is_enum() const override { return true; }
        bool is_class() const override { return false; }
        bool is_template_instance() const override { return false; }


        Res<FundamentalPtr, ErrAsFundamental> as_fundamental() const override { 
            return Err(ErrAsFundamental::NOT_A_FUNDAMENTAL); 
        }
        Res<EnumPtr, ErrAsEnum> as_enum() const override { 
            return Ok(EnumPtr(this)); 
        }
        Res<ClassPtr, ErrAsClass> as_class() const override { 
            return Err(ErrAsClass::NOT_A_CLASS); 
        }
        Res<TemplateInstancePtr, ErrAsTemplateInstance> as_template_instance() const override { 
            return Err(ErrAsTemplateInstance::NOT_A_TEMPLATE_INSTANCE); 
        }

    private:
        std::vector<EnumValue> m_values;
        std::vector<const EnumValue*> m_values_ptrs;

    }; // class Enum

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT EnumValue {
    public:
        friend Enum;

        EnumValue() = default;
        EnumValue(std::string name, size_t value)
            : m_name(std::move(name)), m_value(value) {}

        const std::string& name() const { return m_name; }
        size_t value() const { return m_value; }
        EnumPtr declaring_enum() const { return m_declaring_enum; }

    private:
        std::string m_name;
        size_t m_value = 0;
        EnumPtr m_declaring_enum = 0;

    }; // class EnumValue

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename ENUM>
    class EnumInstance : public Enum, public TypeInstance<ENUM> {
    public:
        EnumInstance(std::string name, std::vector<EnumValue> values);
    }; // class EnumInstance

    //*********************************************************************************************
    template <typename ENUM>
    EnumInstance<ENUM>::EnumInstance(std::string name, std::vector<EnumValue> values)
        : Type(std::move(name), sizeof(ENUM), {}) , Enum(std::move(values)) 
    {}
} // namespace rtti
