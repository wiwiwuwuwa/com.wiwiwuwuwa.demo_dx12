#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/has_constructor_argument_wrapper.hpp>

namespace aiva2
{
    template <typename t_type, typename... t_args>
    constexpr auto has_constructor()
    {
        return std::is_constructible_v<t_type, has_constructor_argument_wrapper_t<t_args>...>;
    }

    template <typename t_type, typename... t_args>
    struct has_constructor_t : public std::integral_constant<bool, has_constructor<t_type, t_args...>()> {};

    template <typename t_type, typename... t_args>
    constexpr auto has_constructor_v = has_constructor_t<t_type, t_args...>::value;
}
