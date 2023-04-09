#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    enum class hlsl_primitive_type_t
    {
        UNKNOWN,
        INT,
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        FLOAT2X2,
        FLOAT3X3,
        FLOAT4X4,
        UINT,
        MAXIMUM,
    };
}
