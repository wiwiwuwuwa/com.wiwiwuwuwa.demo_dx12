#include <pch.h>
#include <aiva2/gpu_res_lib.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/gpu_res.hpp>

namespace aiva2
{
    auto gpu_res_lib_t::get_instance() -> gpu_res_lib_t&
    {
        static auto instance = gpu_res_lib_t{};
        return instance;
    }

    auto gpu_res_lib_t::copy_deep(std::shared_ptr<gpu_res_t> const& res) -> std::shared_ptr<gpu_res_t>
    {
        assert_t::check_bool(res, "(res) is not valid");

        auto const key = copy_deep_key_type{ typeid(*res) };
        auto const val = get_instance().m_copy_deep_funcs.find(key);
        assert_t::check_bool(val != std::cend(get_instance().m_copy_deep_funcs), "(val) is not found");

        return (*val).second(res);
    }

    auto gpu_res_lib_t::copy_shlw(std::shared_ptr<gpu_res_t> const& res) -> std::shared_ptr<gpu_res_t>
    {
        assert_t::check_bool(res, "(res) is not valid");

        auto const key = copy_shlw_key_type{ typeid(*res) };
        auto const val = get_instance().m_copy_shlw_funcs.find(key);
        assert_t::check_bool(val != std::cend(get_instance().m_copy_shlw_funcs), "(val) is not found");

        return (*val).second(res);
    }
}
