#include <pch.h>
#include <aiva2/d3d12_render_output_desc.hpp>

#include <aiva2/buffer_format.hpp>
#include <aiva2/buffer_format_to_dxgi_format.hpp>
#include <aiva2/shader_info.hpp>
#include <aiva2/shader_info_for_meta.hpp>

namespace aiva2
{
    d3d12_render_output_desc_t::d3d12_render_output_desc_t(shader_info_t const& shader_info)
    {
        init_render_targets(shader_info);
        init_depth_stencil(shader_info);
    }

    d3d12_render_output_desc_t::~d3d12_render_output_desc_t()
    {
        shut_depth_stencil();
        shut_render_targets();
    }

    auto d3d12_render_output_desc_t::get_render_targets() const -> std::array<DXGI_FORMAT, 8> const&
    {
        return m_render_targets_arr;
    }

    auto d3d12_render_output_desc_t::num_render_targets() const -> size_t
    {
        return m_render_targets_num;
    }

    void d3d12_render_output_desc_t::init_render_targets(shader_info_t const& shader_info)
    {
        m_render_targets_arr.fill(DXGI_FORMAT_UNKNOWN);
        m_render_targets_num = {};

        auto const render_targets_arr = shader_info.get_meta_block().get_render_target_formats();
        auto const render_targets_num = std::min(std::size(render_targets_arr), std::size(m_render_targets_arr));

        for (auto i = size_t{}; i < render_targets_num; i++)
        {
            m_render_targets_arr[i] = to_dxgi_format(render_targets_arr[i]);
        }

        m_render_targets_num = render_targets_num;
    }

    void d3d12_render_output_desc_t::shut_render_targets()
    {
        m_render_targets_arr.fill(DXGI_FORMAT_UNKNOWN);
        m_render_targets_num = {};
    }

    auto d3d12_render_output_desc_t::get_depth_stencil() const -> DXGI_FORMAT
    {
        return m_depth_stencil;
    }

    void d3d12_render_output_desc_t::init_depth_stencil(shader_info_t const& shader_info)
    {
        if (shader_info.get_meta_block().has_depth_stencil_format())
        {
            m_depth_stencil = to_dxgi_format(shader_info.get_meta_block().get_depth_stencil_format());
        }
        else
        {
            m_depth_stencil = DXGI_FORMAT_UNKNOWN;
        }
    }

    void d3d12_render_output_desc_t::shut_depth_stencil()
    {
        m_depth_stencil = DXGI_FORMAT_UNKNOWN;
    }
}
