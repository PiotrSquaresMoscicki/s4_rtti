#pragma once

#include <cassert>
#include <vector>
#include <map>

#include <core/str/string_id.hpp>

#include "type_instance.hpp"
#include "buffer.hpp"
#include "object.hpp"

namespace rtti {

    using namespace core::str;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Class : public virtual Type {
    public:
        template <typename C, typename D, typename F> friend class FieldInstance;
        template <typename C, typename R, typename... P> friend class MethodInstance;

        enum class ErrMethod {
            METHOD_NOT_FOUND
        };

        const std::vector<const Member*>& members() const { return m_members; }
        const std::vector<const Method*>& methods() const { return m_methods; }
        Res<const Method*, ErrMethod> method(StringId name) const;

        bool is_fundamental() const override { return false; }
        bool is_enum() const override { return false; }
        bool is_class() const override { return true; }
        bool is_template_instance() const override { return false; }

        Res<FundamentalPtr, ErrAsFundamental> as_fundamental() const override { 
            return Err(ErrAsFundamental::NOT_A_FUNDAMENTAL); 
        }
        Res<EnumPtr, ErrAsEnum> as_enum() const override { 
            return Err(ErrAsEnum::NOT_AN_ENUM); 
        }
        Res<ClassPtr, ErrAsClass> as_class() const override { 
            return Ok(ClassPtr(this)); 
        }
        Res<TemplateInstancePtr, ErrAsTemplateInstance> as_template_instance() const override { 
            return Err(ErrAsTemplateInstance::NOT_A_TEMPLATE_INSTANCE); 
        }

    private:
        std::vector<const Member*> m_members;
        std::vector<const Method*> m_methods;
        std::map<StringId, const Method*> m_methods_by_name;

    }; // class Class

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename CLASS>
    class ClassInstance : public Class, public TypeInstance<CLASS> {
    public:
        ClassInstance(std::string name);
        ClassInstance(std::string name, Meta meta);
    }; // class ClassInstance

    //*********************************************************************************************
    template <typename CLASS>
    ClassInstance<CLASS>::ClassInstance(std::string name)
        : Type(std::move(name), sizeof(CLASS), {}) 
    {}

    //*********************************************************************************************
    template <typename CLASS>
    ClassInstance<CLASS>::ClassInstance(std::string name, Meta meta)
        : Type(std::move(name), sizeof(CLASS), std::move(meta))
    {}

} // namespace rtti
