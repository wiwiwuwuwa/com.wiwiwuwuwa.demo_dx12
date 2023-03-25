#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/gpu_res_type.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    template <typename t_gpu_res>
    struct find_gpu_res_type final : public std::integral_constant<gpu_res_type_t, gpu_res_type_t::UNKNOWN> {};
    
    // ----------------------------------------------------

    template <>
    struct find_gpu_res_type<tex_2d_t> final : public std::integral_constant<gpu_res_type_t, gpu_res_type_t::TEXTURE2D> {};

    // ----------------------------------------------------

    template <typename t_gpu_res>
    constexpr auto find_gpu_res_type_v = find_gpu_res_type<t_gpu_res>::value;

    // ----------------------------------------------------
}
