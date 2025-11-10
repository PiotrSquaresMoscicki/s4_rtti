#pragma once

#include <cassert>
#include <type_traits>

#include "type_instance.hpp"
#include "buffer.hpp"
#include "object.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Fundamental : public virtual Type {
    public:
        bool is_fundamental() const override { return true; }
        bool is_enum() const override { return false; }
        bool is_class() const override { return false; }
        bool is_container() const override { return false; }
        bool is_template_instance() const override { return false; }

        Res<FundamentalPtr, ErrAsFundamental> as_fundamental() const override { 
            return Ok(FundamentalPtr(this)); 
        }
        Res<EnumPtr, ErrAsEnum> as_enum() const override { 
            return Err(ErrAsEnum::NOT_AN_ENUM); 
        }
        Res<ClassPtr, ErrAsClass> as_class() const override { 
            return Err(ErrAsClass::NOT_A_CLASS); 
        }
        Res<ContainerPtr, ErrAsContainer> as_container() const override { 
            return Err(ErrAsContainer::NOT_A_CONTAINER); 
        }
        Res<TemplateInstancePtr, ErrAsTemplateInstance> as_template_instance() const override { 
            return Err(ErrAsTemplateInstance::NOT_A_TEMPLATE_INSTANCE); 
        }

    }; // class Fundamental

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    class FundamentalInstance : public Fundamental, public TypeInstance<FUNDAMENTAL> {
    public:
        FundamentalInstance(std::string name);
    }; // class FundamentalInstance

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    FundamentalInstance<FUNDAMENTAL>::FundamentalInstance(std::string name)
        : Type(std::move(name), sizeof(FUNDAMENTAL), {}) 
    {}

} // namespace rtti