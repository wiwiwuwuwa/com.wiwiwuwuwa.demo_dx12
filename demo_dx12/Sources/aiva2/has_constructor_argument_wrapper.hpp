#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    template <typename t_type>
    struct has_constructor_argument_wrapper_t final
    {
        operator t_type() const { return {}; }
    };
}
