#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_format.hpp>
#include <aiva2/enum_utils.hpp>
#include <aiva2/hlsl_primitive_type.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    constexpr void from_string(std::string_view const& str, buffer_format_t& val)
    {
        if (str == "UNKNOWN")
            { val = buffer_format_t::UNKNOWN; return; }

        if (str == "R32_SINT")
            { val = buffer_format_t::R32_SINT; return; }

        if (str == "R32_FLOAT")
            { val = buffer_format_t::R32_FLOAT; return; }

        if (str == "R32G32_FLOAT")
            { val = buffer_format_t::R32G32_FLOAT; return; }

        if (str == "R32G32B32_FLOAT")
            { val = buffer_format_t::R32G32B32_FLOAT; return; }

        if (str == "R32G32B32A32_FLOAT")
            { val = buffer_format_t::R32G32B32A32_FLOAT; return; }

        if (str == "R32_UINT")
            { val = buffer_format_t::R32_UINT; return; }

        if (str == "R8G8B8A8_UNORM")
            { val = buffer_format_t::R8G8B8A8_UNORM; return; }

        if (str == "D32_FLOAT")
            { val = buffer_format_t::D32_FLOAT; return; }

        if (str == "MAXIMUM")
            { val = buffer_format_t::MAXIMUM; return; }

        assert_t::check_bool(false, "failed to convert string to buffer format");
        val = buffer_format_t::UNKNOWN;
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

        if (format == buffer_format_t::MAXIMUM)
            return "MAXIMUM";

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
		from_string(json.get<std::string>(), format);
    }

    // ----------------------------------------------------

    constexpr void from_hlsl_primitive_type(hlsl_primitive_type_t const in_val, buffer_format_t& out_val)
    {
        if (in_val == hlsl_primitive_type_t::UNKNOWN)
            { out_val = buffer_format_t::UNKNOWN; return; }

        if (in_val == hlsl_primitive_type_t::INT)
            { out_val = buffer_format_t::R32_SINT; return; }

        if (in_val == hlsl_primitive_type_t::FLOAT)
            { out_val = buffer_format_t::R32_FLOAT; return; }

        if (in_val == hlsl_primitive_type_t::FLOAT2)
            { out_val = buffer_format_t::R32G32_FLOAT; return; }

        if (in_val == hlsl_primitive_type_t::FLOAT3)
            { out_val = buffer_format_t::R32G32B32_FLOAT; return; }

        if (in_val == hlsl_primitive_type_t::FLOAT4)
            { out_val = buffer_format_t::R32G32B32A32_FLOAT; return; }

        if (in_val == hlsl_primitive_type_t::UINT)
            { out_val = buffer_format_t::R32_UINT; return; }

        if (in_val == hlsl_primitive_type_t::MAXIMUM)
            { out_val = buffer_format_t::MAXIMUM; return; }

        assert_t::check_bool(false, "failed to convert hlsl primitive type to buffer format");
        out_val = buffer_format_t::UNKNOWN;
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

        if (format == buffer_format_t::MAXIMUM)
            return hlsl_primitive_type_t::MAXIMUM;

        assert_t::check_bool(false, "failed to convert buffer format to hlsl primitive type");
        return hlsl_primitive_type_t::UNKNOWN;
    }

    // ----------------------------------------------------
}
