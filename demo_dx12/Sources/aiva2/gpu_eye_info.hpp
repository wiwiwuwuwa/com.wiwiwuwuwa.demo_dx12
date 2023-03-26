#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/struct.hpp>

namespace aiva2
{
    struct gpu_eye_info_t : public implements_t<gpu_eye_info_t, struct_t>
    {
        // ------------------------------------------------
        
    protected:
        gpu_eye_info_t();

        ~gpu_eye_info_t() override;

        // ------------------------------------------------
    };
}
