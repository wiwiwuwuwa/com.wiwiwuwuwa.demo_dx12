#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    constexpr void from_string(std::string_view const& str, bool& val)
    {
        if (str == "true")
            { val = true; return; }

        if (str == "false")
            { val = false; return; }

        assert_t::check_bool(false, "failed to convert string to bool");
        val = false;
    }

    constexpr auto to_string(bool const value)
    {
        return value ? "true" : "false";
    }

    // ----------------------------------------------------
}
