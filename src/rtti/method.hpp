#pragma once

#include <string>
#include <optional>

#include <core/util/res.hpp>

#include "object.hpp"
#include "meta.hpp"
#include "class.hpp"
#include "rtti_fwd.hpp"

namespace rtti {

    using namespace core::util;

    class MethodParam;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Method {
    public:
        enum class ErrCall {
            INVALID_PARAMS_NUM,
            INVALID_PARAM_TYPE,
            CONST_SELF
        };

        Method(std::string name, TypePtr return_type, std::vector<const MethodParam*> params
            , ClassPtr declaring_class, Meta meta)
            : m_name(std::move(name)), m_return_type(return_type), m_params(std::move(params))
            , m_declaring_class(declaring_class), m_attributes(std::move(meta)) 
        {}

        virtual ~Method() = default;

        const std::string& name() const { return m_name; }
        TypePtr return_type() const { return m_return_type; }
        const std::vector<const MethodParam*> params() const { return m_params; }
        ClassPtr declaring_class() const { return m_declaring_class; }
        const Meta& meta() const { return m_attributes; }
        template <typename ATTRIBUTE> const ATTRIBUTE* attribute() const;

        virtual Res<ObjectRef, ErrCall> call(const ObjectRef& self
            , const std::vector<ObjectRef*>& params) const = 0;
        virtual Res<ObjectRef, ErrCall> call(ObjectRef& self
            , const std::vector<ObjectRef*>& params) const = 0;

    private:
        const std::string m_name;
        TypePtr const m_return_type = nullptr;
        std::vector<const MethodParam*> m_params;
        ClassPtr const m_declaring_class = nullptr;
        const Meta m_attributes = {};

    }; // class Method

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT MethodParam {
    public:
        MethodParam(std::string name, TypePtr type) : m_name(std::move(name)), m_type(type) {}

        const std::string& name() const { return m_name; }
        TypePtr type() const { return m_type; }

    private:
        std::string m_name;
        TypePtr m_type;

    }; // class MethodParam

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename CLASS, typename RET, typename... PARAMS>
    class MethodInstance : public Method {
    public:
        using MethodType = RET (CLASS::*)(PARAMS...);

        MethodInstance(Class* instance, std::string name, const std::string& params_names
            , MethodType method, Meta meta);
        MethodInstance(Class* instance, std::string name, const std::string& params_names
            , MethodType method);

        Res<ObjectRef, ErrCall> call(const ObjectRef& self
            , const std::vector<ObjectRef*>& params) const override;
        Res<ObjectRef, ErrCall> call(ObjectRef& self
            , const std::vector<ObjectRef*>& params) const override;

    private:
        static std::string generate_name(std::string name, const std::string& params_names);
        static std::vector<const MethodParam*> generate_params(const std::string& params_names);
        
        MethodType m_method = nullptr;

    }; // class MethodInstance


    //*********************************************************************************************
    template <typename CLASS, typename RET, typename... PARAMS>
    MethodInstance<CLASS, RET, PARAMS...>::MethodInstance(Class* instance, std::string name
        , const std::string& params_names, MethodType method, Meta meta) 
        : Method(generate_name(std::move(name), params_names), static_type<RET>()
            , generate_params(params_names), static_class<CLASS>(), std::move(meta))
        , m_method(method)
    {
        instance->m_methods.push_back(this);
    }

    //*********************************************************************************************
    template <typename CLASS, typename RET, typename... PARAMS>
    MethodInstance<CLASS, RET, PARAMS...>::MethodInstance(Class* instance, std::string name
        , const std::string& params_names, MethodType method) 
        : MethodInstance(instance, std::move(name), params_names, method, Meta{})
    {
    }

    //*********************************************************************************************
    template <typename CLASS, typename RET, typename... PARAMS>
    std::string MethodInstance<CLASS, RET, PARAMS...>::
        generate_name(std::string, const std::string&)
    {
        return "DDD";
    }

    //*********************************************************************************************
    template <typename CLASS, typename RET, typename... PARAMS>
    std::vector<const MethodParam*> MethodInstance<CLASS, RET, PARAMS...>::
        generate_params(const std::string&)
    {
        return {};
    }

    //*********************************************************************************************
    template <typename CLASS, typename RET, typename... PARAMS>
    Res<ObjectRef, Method::ErrCall> MethodInstance<CLASS, RET, PARAMS...>::
        call(const ObjectRef&, const std::vector<ObjectRef*>&) const
    {
        return Err(ErrCall::INVALID_PARAM_TYPE);
    }

    //*********************************************************************************************
    template <typename CLASS, typename RET, typename... PARAMS>
    Res<ObjectRef, Method::ErrCall> MethodInstance<CLASS, RET, PARAMS...>
        ::call(ObjectRef&, const std::vector<ObjectRef*>&) const
    {
        return Err(ErrCall::INVALID_PARAM_TYPE);
    }

//    template <typename CLASS, typename RET, typename... PARAMS>
 //   MethodInstance(std::string, const std::string&, RET (CLASS::*)(PARAMS...)) -> MethodInstance

} // namespace rtti