#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_format.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    constexpr auto from_string(std::string_view const& str)
    {
        if (str == "UNKNOWN")
            return buffer_format_t::UNKNOWN;

        if (str == "R8G8B8A8_UNORM")
            return buffer_format_t::R8G8B8A8_UNORM;

        if (str == "R32G32B32A32_FLOAT")
            return buffer_format_t::R32G32B32A32_FLOAT;

        if (str == "D32_FLOAT")
            return buffer_format_t::D32_FLOAT;

        assert_t::check_bool(false, "failed to convert string to buffer format");
        return buffer_format_t::UNKNOWN;
    }

    constexpr auto to_string(buffer_format_t const format)
    {
        if (format == buffer_format_t::UNKNOWN)
            return "UNKNOWN";

        if (format == buffer_format_t::R8G8B8A8_UNORM)
            return "R8G8B8A8_UNORM";

        if (format == buffer_format_t::R32G32B32A32_FLOAT)
            return "R32G32B32A32_FLOAT";

        if (format == buffer_format_t::D32_FLOAT)
            return "D32_FLOAT";

        assert_t::check_bool(false, "failed to convert buffer format to string");
        return "UNKNOWN";
    }

    // ----------------------------------------------------

    void to_json(nlohmann::json& json, buffer_format_t const& format)
    {
        json = nlohmann::json{ to_string(format) };
    }

    void from_json(nlohmann::json const& json, buffer_format_t& format)
    {
        format = from_string(json.get<std::string>());
    }

    // ----------------------------------------------------
}
