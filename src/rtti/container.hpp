#pragma once

#include <cassert>
#include <vector>
#include <map>

#include <core/str/string_id.hpp>

#include "type.hpp"
#include "buffer.hpp"
#include "object.hpp"

namespace rtti {

    using namespace core::str;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT ContainerElementRef : public ObjectRef {};

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT ContainerIterator {
    public:
        ContainerElementRef operator*() { return {}; }
        ContainerIterator& operator++() { return *this; }
        bool operator!=(const ContainerIterator& other) const { return false; }
    };
        
    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Container : public virtual Type {

        bool is_fundamental() const override { return false; }
        bool is_enum() const override { return false; }
        bool is_class() const override { return false; }
        bool is_template_instance() const override { return false; }

        Res<FundamentalPtr, ErrAsFundamental> as_fundamental() const override { 
            return Err(ErrAsFundamental::NOT_A_FUNDAMENTAL); 
        }
        Res<EnumPtr, ErrAsEnum> as_enum() const override { 
            return Err(ErrAsEnum::NOT_AN_ENUM); 
        }
        Res<ClassPtr, ErrAsClass> as_class() const override { 
            return Err(ErrAsClass::NOT_A_CLASS);
        }
        Res<TemplateInstancePtr, ErrAsTemplateInstance> as_template_instance() const override { 
            return Err(ErrAsTemplateInstance::NOT_A_TEMPLATE_INSTANCE); 
        }

        // class, c-array, tuple, array
        virtual ContainerIterator begin(ObjectRef& obj) const = 0;
        virtual ContainerIterator end(ObjectRef& obj) const = 0;
        virtual size_t length(const ObjectRef& obj) const = 0;
    }; // class Container

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT DynamicContainer : public virtual Container {
    public:
        // map, set
        virtual void erase(ObjectRef& obj, ContainerIterator& it) const = 0;
        virtual void clear(ObjectRef& obj) const = 0;
        virtual void add_copy(ObjectRef& obj, const ObjectRef& value) const = 0;
        virtual void add_move(ObjectRef& obj, ObjectRef&& value) const = 0;
    }; // class DynamicContainer

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT DynamicSequentialContainer : public virtual DynamicContainer {
    public:
        // vector
        virtual void insert_copy(ObjectRef& obj, ContainerIterator& it, const ObjectRef& value) const = 0;
        virtual void insert_move(ObjectRef& obj, ContainerIterator& it, ObjectRef&& value) const = 0;
    }; // class DynamicSequentialContainer

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template<typename FIRST, typename... REST>
    std::string convert_params_to_string() {
        std::string result = static_type<FIRST>::get()->name();
        if constexpr (sizeof...(REST) > 0) {
            result += "," + convert_params_to_string<REST...>();
        }
        return result;
    }
    
    class ContainerInstanceTemplateParameter {
    public:
        ContainerInstanceTemplateParameter(TypePtr type) : m_type(std::move(type)) {}
        const TypePtr& type() const { return m_type; }

    private:
        TypePtr m_type;
    };

    template <typename FULL_TYPE, typename... PARAMS>
    class S4_RTTI_EXPORT ContainerInstance 
        : public virtual TypeInstance<FULL_TYPE>
        , public virtual Container 
    {
    public:
        ContainerInstance(std::string name) 
            : Type(name + "<" + convert_params_to_string<PARAMS...>() + ">"
            , sizeof(FULL_TYPE), {})
        {}

        ContainerIterator begin(ObjectRef& obj) const override { return {}; }
        ContainerIterator end(ObjectRef& obj) const override { return {}; }
        size_t length(const ObjectRef& obj) const override { return 0; }
    }; // class ContainerInstance

} // namespace rtti