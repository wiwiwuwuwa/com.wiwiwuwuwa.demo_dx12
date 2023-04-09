#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    template <typename t_enum>
    class enum_from_string_wrapper_t final
    {
    public:
        using enum_type = t_enum;

    public:
        constexpr enum_from_string_wrapper_t(enum_type const value) : value{ value }
        {

        }

        constexpr operator enum_type() const
        {
            return value;
        }

    private:
        enum_type const value{};
    };

    // ----------------------------------------------------

    template <typename t_enum_wrapper>
    constexpr auto enum_from_string_impl(std::string_view const& value) -> typename t_enum_wrapper::enum_type;

    // ----------------------------------------------------

    template <typename t_enum>
    constexpr auto enum_from_string(std::string_view const& value) -> t_enum
    {
        return enum_from_string_impl<enum_from_string_wrapper_t<t_enum>>(value);
    }

    // ----------------------------------------------------
}
