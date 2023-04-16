#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_cmd_base.hpp>

namespace aiva2
{
    struct gpu_cmd_dispatch_t final : implements_t<gpu_cmd_dispatch_t, gpu_cmd_base_t>
    {
        // ------------------------------------------------

    public:
        gpu_cmd_dispatch_t(engine_t& engine);

        ~gpu_cmd_dispatch_t() override;

    public:
        void execute() override;

    private:
        void execute_resource_barrier() const;

        void execute_set_pipeline_state() const;

        void execute_set_compute_root_signature() const;

        void execute_set_descriptor_heaps() const;

        void execute_set_compute_root_descriptor_table() const;

        void execute_dispatch() const;

        // ------------------------------------------------

    public:
        auto get_material_ptr() const->std::shared_ptr<compute_material_t> const&;

        auto get_material_ref() const->compute_material_t&;

        void set_material_ptr(std::shared_ptr<compute_material_t> const& material);

    private:
        std::shared_ptr<compute_material_t> m_material{};

        // ------------------------------------------------

    public:
        auto get_property_block_ptr() const->std::shared_ptr<material_property_block_t const> const&;

        auto get_property_block_ref() const->material_property_block_t const&;

        void set_property_block_ptr(std::shared_ptr<material_property_block_t const> const& property_block);

    private:
        std::shared_ptr<material_property_block_t const> m_property_block{};

        // ------------------------------------------------

    public:
        auto get_thread_group_count() const->glm::u16vec3 const&;

        void set_thread_group_count(glm::u16vec3 const& thread_group_count);

    private:
        glm::u16vec3 m_thread_group_count{};

        // ------------------------------------------------
    };
}
