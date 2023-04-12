#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_cmd_base.hpp>

namespace aiva2
{
    struct gpu_cmd_clear_render_target_t final : public implements_t<gpu_cmd_clear_render_target_t, gpu_cmd_base_t>
    {
        // ------------------------------------------------

    public:
        gpu_cmd_clear_render_target_t(engine_t& engine);

        ~gpu_cmd_clear_render_target_t() override;

    public:
        void execute() override;

    private:
        void execute_resource_barrier() const;

        void execute_clear_render_target_view() const;

        void execute_clear_depth_stencil_view() const;

        // ------------------------------------------------

    public:
        auto get_render_target_ptr() const->std::shared_ptr<render_target_t const> const&;

        auto get_render_target_ref() const->render_target_t const&;

        void set_render_target_ptr(std::shared_ptr<render_target_t const> const& render_target);

    private:
        std::shared_ptr<render_target_t const> m_render_target{};

        // ------------------------------------------------

    public:
        auto get_clear_color_opt() const->std::optional<glm::vec4> const&;

        auto get_clear_color_ref() const->glm::vec4 const&;

        void set_clear_color_opt(std::optional<glm::vec4> const& clear_color);

    private:
        std::optional<glm::vec4> m_clear_color{};

        // ------------------------------------------------

    public:
        auto get_clear_depth_opt() const->std::optional<float> const&;

        auto get_clear_depth_ref() const->float const&;

        void set_clear_depth_opt(std::optional<float> const& clear_depth);

    private:
        std::optional<float> m_clear_depth{};

        // ------------------------------------------------
    };
}