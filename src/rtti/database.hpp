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

        static Res<TypePtr, ErrRegisterType> register_type(TypePtr type);

    private:
        static std::map<StringId, TypePtr> m_name_to_type;
    }; // class Database

} // namespace rtti