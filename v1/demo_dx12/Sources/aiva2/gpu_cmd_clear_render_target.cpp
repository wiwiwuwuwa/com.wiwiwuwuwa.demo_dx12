#include <pch.h>
#include <aiva2/gpu_cmd_clear_render_target.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/render_target.hpp>

namespace aiva2
{
    gpu_cmd_clear_render_target_t::gpu_cmd_clear_render_target_t(engine_t& engine)
        : impl_type{ engine }
    {
        
    }

    gpu_cmd_clear_render_target_t::~gpu_cmd_clear_render_target_t()
    {
        
    }

    void gpu_cmd_clear_render_target_t::execute()
    {
        execute_resource_barrier();
        execute_clear_render_target_view();
        execute_clear_depth_stencil_view();
    }

    void gpu_cmd_clear_render_target_t::execute_resource_barrier() const
    {
        if (!get_render_target_ptr())
        {
            return;
        }

        for (auto const& resource_barrier : get_render_target_ref().init_for_rendering())
        {
            get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &resource_barrier);
        }
    }

    void gpu_cmd_clear_render_target_t::execute_clear_render_target_view() const
    {
        if (!get_render_target_ptr() || !get_clear_color_opt())
        {
            return;
        }
        
        for (auto i = size_t{}; i < get_render_target_ref().num_color_handle(); i++)
        {
            auto const color_handle = get_render_target_ref().get_color_handle(i);
            if (!color_handle)
            {
                continue;
            }

            get_engine().get_graphic_hardware().get_command_list().ClearRenderTargetView
            (
                /*RenderTargetView*/ (*color_handle),
                /*ColorRGBA*/ glm::value_ptr(get_clear_color_ref()),
                /*NumRects*/ {},
                /*pRects*/ {}
            );
        }
    }

    void gpu_cmd_clear_render_target_t::execute_clear_depth_stencil_view() const
    {
        if (!get_render_target_ptr() || !get_clear_depth_opt())
        {
            return;
        }

        auto const depth_handle = get_render_target_ref().get_depth_handle();
        if (!depth_handle)
        {
            return;
        }

        get_engine().get_graphic_hardware().get_command_list().ClearDepthStencilView
        (
            /*DepthStencilView*/ (*depth_handle),
            /*ClearFlags*/ D3D12_CLEAR_FLAG_DEPTH,
            /*Depth*/ get_clear_depth_ref(),
            /*Stencil*/ {},
            /*NumRects*/ {},
            /*pRects*/ {}
        );
    }

    auto gpu_cmd_clear_render_target_t::get_render_target_ptr() const->std::shared_ptr<render_target_t const> const&
    {
        return m_render_target;
    }

    auto gpu_cmd_clear_render_target_t::get_render_target_ref() const->render_target_t const&
    {
        assert_t::check_bool(m_render_target, "(m_render_target) is not valid");
        return (*m_render_target);
    }

    void gpu_cmd_clear_render_target_t::set_render_target_ptr(std::shared_ptr<render_target_t const> const& render_target)
    {
        m_render_target = render_target;
    }

    auto gpu_cmd_clear_render_target_t::get_clear_color_opt() const->std::optional<glm::vec4> const&
    {
        return m_clear_color;
    }

    auto gpu_cmd_clear_render_target_t::get_clear_color_ref() const->glm::vec4 const&
    {
        assert_t::check_bool(m_clear_color, "(m_clear_color) is not valid");
        return (*m_clear_color);
    }

    void gpu_cmd_clear_render_target_t::set_clear_color_opt(std::optional<glm::vec4> const& clear_color)
    {
        m_clear_color = clear_color;
    }

    auto gpu_cmd_clear_render_target_t::get_clear_depth_opt() const->std::optional<float> const&
    {
        return m_clear_depth;
    }

    auto gpu_cmd_clear_render_target_t::get_clear_depth_ref() const->float const&
    {
        assert_t::check_bool(m_clear_depth, "(m_clear_depth) is not valid");
        return (*m_clear_depth);
    }

    void gpu_cmd_clear_render_target_t::set_clear_depth_opt(std::optional<float> const& clear_depth)
    {
        m_clear_depth = clear_depth;
    }
}
