#pragma once

#include "ctti.hpp"

namespace ctti::attr {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template<typename... Bases>
    class Base {
    public:
        constexpr size_t size() const { return sizeof...(Bases); }

        template <typename T>
        constexpr bool has() const { return false; }

        // @todo implement get for templates 

        template <size_t idx = 0>
        constexpr auto get() const { return get_base_at<idx, Bases...>(); }

    private:
        template <size_t idx, typename T, typename... Args>
        constexpr auto get_base_at() const
        {
            if constexpr (idx == 0)
                return ctti::constecpr_type<T>();
            else
                return get_base_at<Args...>(idx - 1);
        }

    }; // class Base

} // namespace ctti::attr 