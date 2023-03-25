#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_eye_type.hpp>
#include <aiva2/gpu_res_type.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    template <gpu_eye_type_t t_gpu_eye_type, gpu_res_type_t t_gpu_res_type>
    struct find_gpu_eye_info_a;

    // ----------------------------------------------------

    template <>
    struct find_gpu_eye_info_a<gpu_eye_type_t::DEPTH_STENCIL, gpu_res_type_t::TEXTURE2D> final
    {
        using type = dsv_tex_2d_info_t;
    };

    template <>
    struct find_gpu_eye_info_a<gpu_eye_type_t::RENDER_TARGET, gpu_res_type_t::TEXTURE2D> final
    {
        using type = rtv_tex_2d_info_t;
    };

    template <>
    struct find_gpu_eye_info_a<gpu_eye_type_t::UNORDERED_ACCESS, gpu_res_type_t::TEXTURE2D> final
    {
        using type = uav_tex_2d_info_t;
    };

    // ----------------------------------------------------

    template <gpu_eye_type_t t_gpu_eye_type, gpu_res_type_t t_gpu_res_type>
    using find_gpu_eye_info_a_t = typename find_gpu_eye_info_a<t_gpu_eye_type, t_gpu_res_type>::type;

    // ----------------------------------------------------
}
