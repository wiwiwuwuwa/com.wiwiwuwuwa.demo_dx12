#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    template <typename t_enum>
    class parse_enum_wrapper_t final
    {
    public:
        using enum_type = t_enum;

    public:
        constexpr parse_enum_wrapper_t(enum_type const value) : value{ value }
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
    constexpr auto parse_enum_impl(std::string_view const& value) -> typename t_enum_wrapper::enum_type;

    // ----------------------------------------------------

    template <typename t_enum>
    constexpr auto parse_enum(std::string_view const& value) -> t_enum
    {
        return parse_enum_impl<parse_enum_wrapper_t<t_enum>>(value);
    }

    // ----------------------------------------------------
}
