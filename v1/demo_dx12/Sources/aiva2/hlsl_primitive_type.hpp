#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    enum class hlsl_primitive_type_t
    {
        UNKNOWN,
        INT,
        INT2,
        INT3,
        INT4,
        INT2X2,
        INT3X3,
        INT4X4,
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        FLOAT2X2,
        FLOAT3X3,
        FLOAT4X4,
        UINT,
        UINT2,
        UINT3,
        UINT4,
        UINT2X2,
        UINT3X3,
        UINT4X4,
        MAXIMUM,
    };
}
