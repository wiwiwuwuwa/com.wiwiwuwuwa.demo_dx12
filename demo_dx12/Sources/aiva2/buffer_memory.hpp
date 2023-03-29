#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    enum class buffer_memory_t
    {
        UNKNOWN,
        CPU_TO_GPU,
        GPU_ONLY,
        GPU_TO_CPU,
        MAXIMUM,
    };
}
