#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/find_gpu_eye_info_a.hpp>
#include <aiva2/find_gpu_res_type.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    template <gpu_eye_type_t t_gpu_eye_type, typename t_gpu_res>
    struct find_gpu_eye_info_b final
    {
        using type = find_gpu_eye_info_a_t<t_gpu_eye_type, find_gpu_res_type_v<t_gpu_res>>;
    };

    template <gpu_eye_type_t t_gpu_eye_type, typename t_gpu_res>
    using find_gpu_eye_info_b_t = typename find_gpu_eye_info_b<t_gpu_eye_type, t_gpu_res>::type;

    // ----------------------------------------------------
}