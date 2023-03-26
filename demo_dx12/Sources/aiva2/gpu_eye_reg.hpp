#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_eye_lib.hpp>

namespace aiva2
{
    template <typename t_eye>
    struct gpu_eye_reg_t final
    {
        gpu_eye_reg_t()
        {
            gpu_eye_lib_t::reg_eye<t_eye>();
        }
    };
}

#define GPU_EYE_REG(t_eye) \
    namespace \
    { \
        static aiva2::gpu_eye_reg_t<t_eye> t_eye##_reg{}; \
    } \
