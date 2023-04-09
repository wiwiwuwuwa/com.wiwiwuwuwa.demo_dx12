#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    enum class hlsl_semantic_type_t
    {
        UNKNOWN,
        POSITION,
        NORMAL,
        TEXCOORD,
        COLOR,
        SV_POSITION,
        SV_TARGET,
        MAXIMUM,
    };
}
