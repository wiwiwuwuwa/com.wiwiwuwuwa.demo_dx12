#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    enum class gpu_eye_type_t
    {
        UNKNOWN,
        CONSTANT_BUFFER,
        SHADER_RESOURCE,
        UNORDERED_ACCESS,
        SAMPLER,
        RENDER_TARGET,
        DEPTH_STENCIL,
        MAXIMUM,
    };
}
