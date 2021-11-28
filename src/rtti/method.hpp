#pragma once

#include <string>
#include <optional>

#include <core/util/res.hpp>

#include "object.hpp"
#include "attributes.hpp"
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

        Method(std::string name, const Type* return_type, std::vector<const MethodParam*> params
            , const Class* declaring_class, Attributes attributes)
            : m_name(std::move(name)), m_return_type(return_type), m_params(std::move(params))
            , m_declaring_class(declaring_class), m_attributes(std::move(attributes)) 
        {}

        virtual ~Method() = default;

        const std::string& name() const { return m_name; }
        const Type* return_type() const { return m_return_type; }
        const std::vector<const MethodParam*> params() const { return m_params; }
        const Class* declaring_class() const { return m_declaring_class; }
        const Attributes& attributes() const { return m_attributes; }
        template <typename ATTRIBUTE> const ATTRIBUTE* attribute() const;

        // virtual Res<std::optional<ObjectRef>, ErrCall> call(const ObjectRef& self
        //     , const std::vector<ObjectRef*>& params) const = 0;
        // virtual Res<std::optional<ObjectRef>, ErrCall> call(ObjectRef& self
        //     , const std::vector<ObjectRef*>& params) const = 0;

    private:
        const std::string m_name;
        const Type* const m_return_type = nullptr;
        std::vector<const MethodParam*> m_params;
        const Class* const m_declaring_class = nullptr;
        const Attributes m_attributes = {};

    }; // class Method

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT MethodParam {
    public:
        MethodParam(std::string name, const Type* type) : m_name(std::move(name)), m_type(type) {}

        const std::string& name() const { return m_name; }
        const Type* type() const { return m_type; }

    private:
        std::string m_name;
        const Type* m_type;

    }; // class MethodParam

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS, typename RET, typename... PARAMS>
    class S4_RTTI_EXPORT MethodInstance : public Method {
    public:
        using MethodType = RET (CLASS::*)(PARAMS...);

        MethodInstance(std::string name, const std::string& params_names, MethodType method
            , void(DECLARING_CLASS::*)());
        MethodInstance(std::string name, const std::string& params_names, MethodType method
            , void(DECLARING_CLASS::*)(), Attributes attributes);

        // Res<ObjectRef, ErrCall> call(const ObjectRef& self
        //     , const std::vector<ObjectRef*>& params) const override;
        // Res<ObjectRef, ErrCall> call(ObjectRef& self
        //     , const std::vector<ObjectRef*>& params) const override;

    private:
        static std::string generate_name(std::string name, const std::string& params_names);
        static std::vector<const MethodParam*> generate_params(const std::string& params_names);
        
        MethodType m_method = nullptr;

    }; // class MethodInstance

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS, typename RET, typename... PARAMS>
    MethodInstance<CLASS, DECLARING_CLASS, RET, PARAMS...>::MethodInstance(std::string name
        , const std::string& params_names, MethodType method, void(DECLARING_CLASS::*)()
        , Attributes attributes) 
        : Method(generate_name(std::move(name), params_names), static_type<RET>()
            , generate_params(params_names), static_class<CLASS>(), std::move(attributes))
        , m_method(method)
    {
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS, typename RET, typename... PARAMS>
    MethodInstance<CLASS, DECLARING_CLASS, RET, PARAMS...>::MethodInstance(std::string name
        , const std::string& params_names, MethodType method, void(DECLARING_CLASS::*dummy)()) 
        : MethodInstance(std::move(name), params_names, method, dummy, Attributes{})
    {
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS, typename RET, typename... PARAMS>
    std::string MethodInstance<CLASS, DECLARING_CLASS, RET, PARAMS...>::
        generate_name(std::string, const std::string&)
    {
        return "DDD";
    }

    //*********************************************************************************************
    template <typename CLASS, typename DECLARING_CLASS, typename RET, typename... PARAMS>
    std::vector<const MethodParam*> MethodInstance<CLASS, DECLARING_CLASS, RET, PARAMS...>::
        generate_params(const std::string&)
    {
        return {};
    }

    // //*********************************************************************************************
    // template <typename CLASS, typename DECLARING_CLASS, typename RET, typename... PARAMS>
    // Res<ObjectRef, Method::ErrCall> MethodInstance<CLASS, DECLARING_CLASS, RET, PARAMS...>
    //     ::call(const ObjectRef& self, const std::vector<ObjectRef*>& params) const
    // {
    //     return Err(ErrCall::INVALID_PARAM_TYPE);
    // }

    // //*********************************************************************************************
    // template <typename CLASS, typename DECLARING_CLASS, typename RET, typename... PARAMS>
    // Res<ObjectRef, Method::ErrCall> MethodInstance<CLASS, DECLARING_CLASS, RET, PARAMS...>
    //     ::call(ObjectRef& self, const std::vector<ObjectRef*>& params) const
    // {
    //     return Err(ErrCall::INVALID_PARAM_TYPE);
    // }

//    template <typename CLASS, typename DECLARING_CLASS, typename RET, typename... PARAMS>
 //   MethodInstance(std::string, const std::string&, RET (CLASS::*)(PARAMS...)) -> MethodInstance

} // namespace rtti