#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_cmd_base.hpp>

namespace aiva2
{
    struct gpu_cmd_draw_proc_t final : implements_t<gpu_cmd_draw_proc_t, gpu_cmd_base_t>
    {
        // ------------------------------------------------

    public:
        gpu_cmd_draw_proc_t(engine_t& engine);

        ~gpu_cmd_draw_proc_t() override;

    public:
        void execute() override;

    public:
        void execute_resource_barrier() const;

        void execute_set_pipeline_state() const;

        void execute_set_graphic_root_signature() const;

        void execute_set_descriptor_heaps() const;

        void execute_set_graphics_root_descriptor_table() const;

        void execute_ia_set_primitive_topology() const;

        void execute_draw_instanced() const;

        // ------------------------------------------------

    public:
        auto get_material_ptr() const->std::shared_ptr<graphic_material_t> const&;

        auto get_material_ref() const->graphic_material_t&;

        void set_material_ptr(std::shared_ptr<graphic_material_t> const& material);

    private:
        std::shared_ptr<graphic_material_t> m_material{};

        // ------------------------------------------------

    public:
        auto get_property_block_ptr() const->std::shared_ptr<material_property_block_t const> const&;

        auto get_property_block_ref() const->material_property_block_t const&;

        void set_property_block_ptr(std::shared_ptr<material_property_block_t const> const& property_block);

    private:
        std::shared_ptr<material_property_block_t const> m_property_block{};

        // ------------------------------------------------

    public:
        auto get_vertices_count() const->size_t;

        void set_vertices_count(size_t const vertices_count);

    private:
        size_t m_vertices_count{};

        // ------------------------------------------------

    public:
        auto get_instances_count() const->size_t;

        void set_instances_count(size_t const instances_count);

    private:
        size_t m_instances_count{};

        // ------------------------------------------------
    };
}
