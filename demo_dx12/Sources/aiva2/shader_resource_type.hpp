#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    enum class shader_resource_type_t
    {
        UNKNOWN,
        CONSTANT_BUFFER,
        RW_TEXTURE_2D,
        MAXIMUM,
    };
}