#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/enum_utils.hpp>
#include <aiva2/shader_resource_type.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    constexpr void from_hlsl(std::string_view const& in_val, shader_resource_type_t& out_val)
    {
        if (in_val == "ConstantBuffer")
            { out_val = shader_resource_type_t::CONSTANT_BUFFER; return; }

        if (in_val == "RWTexture2D")
            { out_val = shader_resource_type_t::RW_TEXTURE_2D; return; }

        assert_t::check_bool(false, "failed to parse shader resource type from hlsl");
        out_val = shader_resource_type_t::UNKNOWN;
    }
    
    // ----------------------------------------------------

    constexpr auto to_hlsl(shader_resource_type_t const type)
    {
        if (type == shader_resource_type_t::CONSTANT_BUFFER)
            return "ConstantBuffer";

        if (type == shader_resource_type_t::RW_TEXTURE_2D)
            return "RWTexture2D";

        assert_t::check_bool(false, "failed to parse shader resource type to hlsl");
        return "";
    }

    // ----------------------------------------------------
}
