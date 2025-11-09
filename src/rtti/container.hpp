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
    class S4_RTTI_EXPORT Container : public virtual Type {
    }; // class Container
    
} // namespace rtti