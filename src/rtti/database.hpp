#pragma once

#include <map>

#include <core/str/string_id.hpp>
#include <core/util/res.hpp>

#include "rtti_fwd.hpp"

namespace rtti {

    using namespace core::str;
    using namespace core::util;

    //*********************************************************************************************
    class S4_RTTI_EXPORT Database {
    public:
        enum class ErrRegisterType {
            UNDEFINED
        };

        enum class ErrRegisterFunction {
            UNDEFINED
        };

        static Res<TypePtr, ErrRegisterType> register_type(TypePtr type);
        static Res<const Function*, ErrRegisterFunction> register_function(const Function* function);

    private:
        static std::map<StringId, TypePtr>& get_name_to_type();
        static std::map<StringId, const Function*>& get_name_to_function();
    }; // class Database

} // namespace rtti