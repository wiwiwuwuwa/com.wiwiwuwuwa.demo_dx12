#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_res.hpp>
#include <aiva2/gpu_cmd_copy_resource.hpp>
#include <aiva2/graphic_executor.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    template <typename t_res>
    auto copy_deep(std::shared_ptr<t_res> const& res_src) -> std::shared_ptr<t_res>
    {
        if (!res_src) return {};
        
        auto const res_dst = std::make_shared<t_res>((*res_src).get_engine(), (*res_src).get_info());
        if (!res_dst) return {};

        auto const cmd_cpy = std::make_shared<gpu_cmd_copy_resource_t>((*res_src).get_engine());
        if (!cmd_cpy) return {};

        (*cmd_cpy).set_src(res_src);
        (*cmd_cpy).set_dst(res_dst);

        (*res_src).get_engine().get_graphic_executor().execute_command(cmd_cpy);
        
        return res_dst;
    }

    template <typename t_res>
    auto copy_shlw(std::shared_ptr<t_res> const& res_obj) -> std::shared_ptr<t_res>
    {
        if (!res_obj) return {};

        auto const res_raw = res_obj.get_resource();
        if (!res_raw) return {};

        return std::make_shared<t_res>((*res_obj).get_engine(), res_raw);
    }

    // ----------------------------------------------------
}
