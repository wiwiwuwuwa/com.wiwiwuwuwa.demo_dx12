#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/object.hpp>

namespace aiva2
{
    struct d3d12_render_output_desc_t final : public implements_t<d3d12_render_output_desc_t, object_t>
    {
        // ------------------------------------------------

    public:
        d3d12_render_output_desc_t(shader_info_t const& shader_info);

        ~d3d12_render_output_desc_t() override;

        // ------------------------------------------------

    public:
        auto get_render_targets() const -> std::array<DXGI_FORMAT, 8> const&;

        auto num_render_targets() const -> size_t;

    private:
        void init_render_targets(shader_info_t const& shader_info);

        void shut_render_targets();

    private:
        std::array<DXGI_FORMAT, 8> m_render_targets_arr{};

        size_t m_render_targets_num{};

        // ------------------------------------------------

    public:
        auto get_depth_stencil() const -> DXGI_FORMAT;

    private:
        void init_depth_stencil(shader_info_t const& shader_info);

        void shut_depth_stencil();

    private:
        DXGI_FORMAT m_depth_stencil{};

        // ------------------------------------------------
    };
}
