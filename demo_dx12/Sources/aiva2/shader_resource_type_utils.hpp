#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/enum_utils.hpp>
#include <aiva2/shader_resource_type.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    constexpr auto from_hlsl(std::string_view const& hlsl)
    {
        if (hlsl == "ConstantBuffer")
            return shader_resource_type_t::CONSTANT_BUFFER;

        if (hlsl == "RWTexture2D")
            return shader_resource_type_t::RW_TEXTURE_2D;

        assert_t::check_bool(false, "failed to parse shader resource type from hlsl");
        return shader_resource_type_t::UNKNOWN;
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
