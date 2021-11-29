#pragma once

namespace ctti {

    template <typename TYPE>
    constexpr auto constexpr_type() { return TYPE::template constexpr_type<TYPE>(); }

} // namespace ctti