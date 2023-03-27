#include <pch.h>
#include <aiva2/gpu_eye_lib.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/gpu_eye.hpp>
#include <aiva2/gpu_eye_info.hpp>
#include <aiva2/gpu_res.hpp>

namespace aiva2
{
    auto gpu_eye_lib_t::get_instance() -> gpu_eye_lib_t&
    {
        static auto instance = gpu_eye_lib_t{};
        return instance;
    }

    auto gpu_eye_lib_t::get_res(std::shared_ptr<gpu_eye_t> const& eye) -> std::shared_ptr<gpu_res_t>
    {
        assert_t::check_bool(eye, "eye is not valid");

        auto const key = get_res_key_type{ typeid(*eye) };
        auto const val = get_instance().m_get_res_funcs.find(key);
        assert_t::check_bool(val != std::cend(get_instance().m_get_res_funcs), "val is not found");

        return (*val).second(eye);
    }

    auto gpu_eye_lib_t::new_eye(engine_t& engine, std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info) -> std::shared_ptr<gpu_eye_t>
    {
        assert_t::check_bool(resource, "resource is not valid");
        assert_t::check_bool(info, "info is not valid");

        auto const key = new_eye_key_type{ typeid(*resource), typeid(*info) };
        auto const val = get_instance().m_new_eye_funcs.find(key);
        assert_t::check_bool(val != std::cend(get_instance().m_new_eye_funcs), "val is not found");

        return (*val).second(engine, resource, info);
    }
}
