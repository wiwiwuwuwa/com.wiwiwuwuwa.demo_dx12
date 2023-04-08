#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    constexpr auto from_string(std::string_view const& str)
    {
        if (str == "true")
            return true;

        if (str == "false")
            return false;

        assert_t::check_bool(false, "failed to convert string to bool");
        return false;
    }

    constexpr auto to_string(bool const value)
    {
        return value ? "true" : "false";
    }

    // ----------------------------------------------------
}
