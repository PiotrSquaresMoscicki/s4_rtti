#pragma once

#include <vector>

#include "rtti_fwd.hpp"
#include "object.hpp"

namespace rtti {

    class FunctionParam;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Function {
    public:
        enum class ErrCall {
            INVALID_PARAMS_NUM,
            INVALID_PARAM_TYPE,
        };

        Function(const std::string& name_no_params, TypePtr return_type
            , std::vector<FunctionParam> params, Meta meta)

            : m_name(generate_name(name_no_params, params)), m_return_type(return_type)
            , m_params(std::move(params)), m_meta(std::move(meta)) 
        {}

        virtual ~Function() = default;

        const std::string& name() const { return m_name; }
        TypePtr return_type() const { return m_return_type; }
        const std::vector<const FunctionParam*> params() const { return m_params_ptrs; }
        const Meta& meta() const { return m_meta; }

        virtual Res<ObjectRef, ErrCall> call(const std::vector<ObjectRef*>& params) const = 0;

    protected:
        static std::string generate_name(const std::string& name
            , const std::vector<FunctionParam> params) 
        { 
            return ""; 
        }

        template <typename... PARAMS>
        static std::vector<FunctionParam> generate_params(const std::string params_names) {
            return {}; 
        }

    private:
        const std::string m_name;
        TypePtr const m_return_type = nullptr;
        const std::vector<FunctionParam> m_params;
        const std::vector<const FunctionParam*> m_params_ptrs;
        const Meta m_meta = {};

    }; // class Function

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT FunctionParam {
    public:
        FunctionParam(std::string name, TypePtr type) : m_name(std::move(name)), m_type(type) {}

        const std::string& name() const { return m_name; }
        TypePtr type() const { return m_type; }

    private:
        const std::string m_name;
        const TypePtr m_type;

    }; // class FunctionParam

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename RET, typename... PARAMS>
    class FunctionInstance : public Function {
    public:
        using FunctionType = RET (*)(PARAMS...);

        FunctionInstance(const std::string& name_no_params, const std::string& params_names
            , FunctionType function, Meta meta = {});

        Res<ObjectRef, ErrCall> call(const std::vector<ObjectRef*>& params) const override;
    
    private:
        FunctionType m_function = nullptr;

    }; // class FunctionInstance

    //*********************************************************************************************
    template <typename RET, typename... PARAMS>
    FunctionInstance<RET, PARAMS...>::FunctionInstance(const std::string& name_no_params
        , const std::string& params_names, FunctionType function, Meta meta)
    
        : Function(name_no_params, static_type<RET>(), generate_params<PARAMS...>(params_names)
            , std::move(meta))
        , m_function(function)
    {
    }

    //*********************************************************************************************
    template <typename RET, typename... PARAMS>
    Res<ObjectRef, Function::ErrCall> FunctionInstance<RET, PARAMS...>::call(
        const std::vector<ObjectRef*>& params) const
    {
        return Err(ErrCall::INVALID_PARAMS_NUM);
    }

} // namespace rtti