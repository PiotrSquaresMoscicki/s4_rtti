#pragma once

#include <string>
#include <optional>

#include <core/util/res.hpp>

#include "function.hpp"
#include "object.hpp"
#include "meta.hpp"
#include "class.hpp"
#include "rtti_fwd.hpp"

namespace rtti {

    using namespace core::util;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Method : public Function {
    public:
        enum class ErrCall {
            INVALID_PARAMS_NUM,
            INVALID_PARAM_TYPE,
            CONST_SELF
        };

        Method(const std::string& name_no_params, TypePtr return_type
            , std::vector<FunctionParam> params, ClassPtr declaring_class, Meta meta)

            : Function(name_no_params, return_type, std::move(params), std::move(meta))
            , m_declaring_class(declaring_class) 
        {}

        ClassPtr declaring_class() const { return m_declaring_class; }

        virtual Res<ObjectRef, ErrCall> call(const ObjectRef& self
            , const std::vector<ObjectRef*>& params) const = 0;
        virtual Res<ObjectRef, ErrCall> call(ObjectRef& self
            , const std::vector<ObjectRef*>& params) const = 0;

    private:
        const ClassPtr m_declaring_class;

    }; // class Method

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename CLASS, typename RET, typename... PARAMS>
    class MethodInstance : public Method {
    public:
        using MethodType = RET (CLASS::*)(PARAMS...);

        MethodInstance(Class* instance, const std::string& name_no_params
            , const std::string& params_names, MethodType method, Meta meta = {});

        Res<ObjectRef, ErrCall> call(const ObjectRef& self
            , const std::vector<ObjectRef*>& params) const override;
        Res<ObjectRef, ErrCall> call(ObjectRef& self
            , const std::vector<ObjectRef*>& params) const override;

    private:
        MethodType m_method = nullptr;

    }; // class MethodInstance

    //*********************************************************************************************
    template <typename CLASS, typename RET, typename... PARAMS>
    MethodInstance<CLASS, RET, PARAMS...>::MethodInstance(Class* instance
        , const std::string& name_no_params, const std::string& params_names, MethodType method
        , Meta meta) 

        : Method(name_no_params, static_type<RET>(), generate_params<PARAMS...>(params_names)
            , static_class<CLASS>(), std::move(meta))
        , m_method(method)
    {
        instance->m_methods.push_back(this);
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

} // namespace rtti