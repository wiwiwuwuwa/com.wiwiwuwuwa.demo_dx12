#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    struct gpu_eye_lib_t final
    {
        // ------------------------------------------------

    private:
        gpu_eye_lib_t() = default;

        // ------------------------------------------------

    public:
        static auto new_eye(std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info)->std::shared_ptr<gpu_eye_t>;

        // ------------------------------------------------
    };
};