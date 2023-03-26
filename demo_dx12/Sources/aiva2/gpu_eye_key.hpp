#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    struct gpu_eye_key_t final
    {
        // ------------------------------------------------

        std::type_index type_of_resource{ typeid(void) };

        std::type_index type_of_info{ typeid(void) };

        // ------------------------------------------------
    };
}
