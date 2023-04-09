#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/hlsl_primitive_type.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    constexpr void from_string(std::string_view const& str, hlsl_primitive_type_t& val)
    {
        if (str == "UNKNOWN")
            { val = hlsl_primitive_type_t::UNKNOWN; return; }

        if (str == "int")
            { val = hlsl_primitive_type_t::INT; return; }

        if (str == "float")
            { val = hlsl_primitive_type_t::FLOAT; return; }

        if (str == "float2")
            { val = hlsl_primitive_type_t::FLOAT2; return; }

        if (str == "float3")
            { val = hlsl_primitive_type_t::FLOAT3; return; }

        if (str == "float4")
            { val = hlsl_primitive_type_t::FLOAT4; return; }

        if (str == "float2x2")
            { val = hlsl_primitive_type_t::FLOAT2X2; return; }

        if (str == "float3x3")
            { val = hlsl_primitive_type_t::FLOAT3X3; return; }

        if (str == "float4x4")
            { val = hlsl_primitive_type_t::FLOAT4X4; return; }

        if (str == "uint")
            { val = hlsl_primitive_type_t::UINT; return; }

        if (str == "MAXIMUM")
            { val = hlsl_primitive_type_t::MAXIMUM; return; }

        assert_t::check_bool(false, "failed to convert string to hlsl primitive type");
        val = hlsl_primitive_type_t::UNKNOWN;
    }

    constexpr auto to_string(hlsl_primitive_type_t const type)
    {
        if (type == hlsl_primitive_type_t::UNKNOWN)
            return "UNKNOWN";

        if (type == hlsl_primitive_type_t::INT)
            return "int";

        if (type == hlsl_primitive_type_t::FLOAT)
            return "float";

        if (type == hlsl_primitive_type_t::FLOAT2)
            return "float2";

        if (type == hlsl_primitive_type_t::FLOAT3)
            return "float3";

        if (type == hlsl_primitive_type_t::FLOAT4)
            return "float4";

        if (type == hlsl_primitive_type_t::FLOAT2X2)
            return "float2x2";

        if (type == hlsl_primitive_type_t::FLOAT3X3)
            return "float3x3";

        if (type == hlsl_primitive_type_t::FLOAT4X4)
            return "float4x4";

        if (type == hlsl_primitive_type_t::UINT)
            return "uint";

        if (type == hlsl_primitive_type_t::MAXIMUM)
            return "MAXIMUM";

        assert_t::check_bool(false, "failed to convert hlsl primitive type to string");
        return "UNKNOWN";
    }

    // ----------------------------------------------------

    void to_json(nlohmann::json& json, hlsl_primitive_type_t const& type)
    {
        json = to_string(type);
    }

    void from_json(nlohmann::json const& json, hlsl_primitive_type_t& type)
    {
		from_string(json.get<std::string>(), type);
    }

    // ----------------------------------------------------
}
