#pragma once

#include <map>

#include <core/str/string_id.hpp>
#include <core/util/res.hpp>

#include "rtti_fwd.hpp"

namespace rtti {

    using namespace core::str;
    using namespace core::util;

    //*********************************************************************************************
    class Database {
    public:
        enum class ErrRegisterType {
            UNDEFINED
        };

        static Res<const Type*, ErrRegisterType> register_type(const Type* type);

    private:
        static std::map<StringId, const Type*> m_name_to_type;
    }; // class Database

} // namespace rtti