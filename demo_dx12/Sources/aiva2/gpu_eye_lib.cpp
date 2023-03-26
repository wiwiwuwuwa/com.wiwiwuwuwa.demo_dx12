#include <pch.h>
#include <aiva2/gpu_eye_lib.hpp>

#include <aiva2/assert.hpp>

namespace aiva2
{
    auto gpu_eye_lib_t::new_eye(std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info)->std::shared_ptr<gpu_eye_t>
    {
        assert_t::check_bool(false, "TODO: IMPLEMENT ME");
        return {};
    }
}
