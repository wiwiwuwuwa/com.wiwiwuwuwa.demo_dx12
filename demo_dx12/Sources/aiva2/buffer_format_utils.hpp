#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_format.hpp>
#include <aiva2/hlsl_primitive_type.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    constexpr auto from_string(std::string_view const& str)
    {
        if (str == "UNKNOWN")
            return buffer_format_t::UNKNOWN;

        if (str == "R32_SINT")
            return buffer_format_t::R32_SINT;

        if (str == "R32_FLOAT")
            return buffer_format_t::R32_FLOAT;

        if (str == "R32G32_FLOAT")
            return buffer_format_t::R32G32_FLOAT;

        if (str == "R32G32B32_FLOAT")
            return buffer_format_t::R32G32B32_FLOAT;

        if (str == "R32G32B32A32_FLOAT")
            return buffer_format_t::R32G32B32A32_FLOAT;

        if (str == "R32_UINT")
            return buffer_format_t::R32_UINT;

        if (str == "R8G8B8A8_UNORM")
            return buffer_format_t::R8G8B8A8_UNORM;

        if (str == "D32_FLOAT")
            return buffer_format_t::D32_FLOAT;

        assert_t::check_bool(false, "failed to convert string to buffer format");
        return buffer_format_t::UNKNOWN;
    }

    constexpr auto to_string(buffer_format_t const format)
    {
        if (format == buffer_format_t::UNKNOWN)
            return "UNKNOWN";

        if (format == buffer_format_t::R32_SINT)
            return "R32_SINT";

        if (format == buffer_format_t::R32_FLOAT)
            return "R32_FLOAT";

        if (format == buffer_format_t::R32G32_FLOAT)
            return "R32G32_FLOAT";

        if (format == buffer_format_t::R32G32B32_FLOAT)
            return "R32G32B32_FLOAT";

        if (format == buffer_format_t::R32G32B32A32_FLOAT)
            return "R32G32B32A32_FLOAT";

        if (format == buffer_format_t::R32_UINT)
            return "R32_UINT";

        if (format == buffer_format_t::R8G8B8A8_UNORM)
            return "R8G8B8A8_UNORM";

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

    constexpr auto from_hlsl_primitive_type(hlsl_primitive_type_t const type)
    {
        if (type == hlsl_primitive_type_t::UNKNOWN)
            return buffer_format_t::UNKNOWN;

        if (type == hlsl_primitive_type_t::INT)
            return buffer_format_t::R32_SINT;

        if (type == hlsl_primitive_type_t::FLOAT)
            return buffer_format_t::R32_FLOAT;

        if (type == hlsl_primitive_type_t::FLOAT2)
            return buffer_format_t::R32G32_FLOAT;

        if (type == hlsl_primitive_type_t::FLOAT3)
            return buffer_format_t::R32G32B32_FLOAT;

        if (type == hlsl_primitive_type_t::FLOAT4)
            return buffer_format_t::R32G32B32A32_FLOAT;

        if (type == hlsl_primitive_type_t::UINT)
            return buffer_format_t::R32_UINT;

        assert_t::check_bool(false, "failed to convert hlsl primitive type to buffer format");
        return buffer_format_t::UNKNOWN;
    }

    constexpr auto to_hlsl_primitive_type(buffer_format_t const format)
    {
        if (format == buffer_format_t::UNKNOWN)
            return hlsl_primitive_type_t::UNKNOWN;

        if (format == buffer_format_t::R32_SINT)
            return hlsl_primitive_type_t::INT;

        if (format == buffer_format_t::R32_FLOAT)
            return hlsl_primitive_type_t::FLOAT;

        if (format == buffer_format_t::R32G32_FLOAT)
            return hlsl_primitive_type_t::FLOAT2;

        if (format == buffer_format_t::R32G32B32_FLOAT)
            return hlsl_primitive_type_t::FLOAT3;

        if (format == buffer_format_t::R32G32B32A32_FLOAT)
            return hlsl_primitive_type_t::FLOAT4;

        if (format == buffer_format_t::R32_UINT)
            return hlsl_primitive_type_t::UINT;

        assert_t::check_bool(false, "failed to convert buffer format to hlsl primitive type");
        return hlsl_primitive_type_t::UNKNOWN;
    }

    // ----------------------------------------------------
}
