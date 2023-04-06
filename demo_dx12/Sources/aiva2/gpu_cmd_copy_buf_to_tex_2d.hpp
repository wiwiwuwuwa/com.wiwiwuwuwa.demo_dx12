#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_cmd_base.hpp>

namespace aiva2
{
    struct gpu_cmd_copy_buf_to_tex_2d_t final : public implements_t<gpu_cmd_copy_buf_to_tex_2d_t, gpu_cmd_base_t>
    {
        // ------------------------------------------------

    public:
        gpu_cmd_copy_buf_to_tex_2d_t(engine_t& engine);

        ~gpu_cmd_copy_buf_to_tex_2d_t() override;

    public:
        void execute() override;

    private:
        void execute_resource_barrier();

        void execute_copy_texture_region();

        // ------------------------------------------------
        
    public:
        auto get_src() const -> std::shared_ptr<buf_t> const&;

        void set_src(std::shared_ptr<buf_t> const& src);

    private:
        std::shared_ptr<buf_t> m_src{};

        // ------------------------------------------------

    public:
        auto get_dst() const -> std::shared_ptr<tex_2d_t> const&;

        void set_dst(std::shared_ptr<tex_2d_t> const& dst);

    private:
        std::shared_ptr<tex_2d_t> m_dst{};

        // ------------------------------------------------
    };
}