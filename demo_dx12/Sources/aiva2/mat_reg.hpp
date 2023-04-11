#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/mat_lib.hpp>

namespace aiva2
{
    template <typename t_mat>
    struct mat_reg_t final
    {
        mat_reg_t()
        {
            mat_lib_t::reg_mat<t_mat>();
        }
    };
}

#define MAT_REG(t_mat) \
    namespace \
    { \
        static aiva2::mat_reg_t<t_mat> t_mat##_reg{}; \
    } \
