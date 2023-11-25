#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/shader_base.hpp>

namespace aiva2
{
    struct graphic_shader_t final : public implements_t<graphic_shader_t, shader_base_t>
    {
        // ------------------------------------------------

    public:
        graphic_shader_t(engine_t& engine, boost::span<std::byte const> const& binary);

        ~graphic_shader_t() override;

        // ------------------------------------------------

    public:
        auto get_pipeline_state_ref() const->ID3D12PipelineState&;

        auto get_pipeline_state_ptr() const->winrt::com_ptr<ID3D12PipelineState> const&;

    private:
        void init_pipeline_state();

        void shut_pipeline_state();

    private:
        winrt::com_ptr<ID3D12PipelineState> m_pipeline_state{};

        // ------------------------------------------------
    };
}
