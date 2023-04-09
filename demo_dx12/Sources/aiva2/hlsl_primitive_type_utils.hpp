#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/enum_from_string.hpp>
#include <aiva2/hlsl_primitive_type.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    template <>
	constexpr auto enum_from_string_impl<enum_from_string_wrapper_t<hlsl_primitive_type_t>>(std::string_view const& str) -> hlsl_primitive_type_t
    {
        if (str == "UNKNOWN")
            return hlsl_primitive_type_t::UNKNOWN;

        if (str == "int")
            return hlsl_primitive_type_t::INT;

        if (str == "float")
            return hlsl_primitive_type_t::FLOAT;

        if (str == "float2")
            return hlsl_primitive_type_t::FLOAT2;

        if (str == "float3")
            return hlsl_primitive_type_t::FLOAT3;

        if (str == "float4")
            return hlsl_primitive_type_t::FLOAT4;

        if (str == "float2x2")
            return hlsl_primitive_type_t::FLOAT2X2;

        if (str == "float3x3")
            return hlsl_primitive_type_t::FLOAT3X3;

        if (str == "float4x4")
            return hlsl_primitive_type_t::FLOAT4X4;

        if (str == "uint")
            return hlsl_primitive_type_t::UINT;

        if (str == "MAXIMUM")
            return hlsl_primitive_type_t::MAXIMUM;

        assert_t::check_bool(false, "failed to convert string to hlsl primitive type");
        return hlsl_primitive_type_t::UNKNOWN;
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
		type = enum_from_string<hlsl_primitive_type_t>(json.get<std::string>());
    }

    // ----------------------------------------------------
}
