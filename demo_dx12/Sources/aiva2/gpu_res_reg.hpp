#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_res_lib.hpp>

namespace aiva2
{
    template <typename t_res>
    struct gpu_res_reg_t final
    {
        gpu_res_reg_t()
        {
            gpu_res_lib_t::reg_res<t_res>();
        }
    };
}

#define GPU_RES_REG(t_res) \
    namespace \
    { \
        static aiva2::gpu_res_reg_t<t_res> t_res##_reg{}; \
    } \
