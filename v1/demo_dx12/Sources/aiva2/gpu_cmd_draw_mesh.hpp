#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_cmd_base.hpp>

namespace aiva2
{
    struct gpu_cmd_draw_mesh_t final : implements_t<gpu_cmd_draw_mesh_t, gpu_cmd_base_t>
    {
        // ------------------------------------------------

    public:
        gpu_cmd_draw_mesh_t(engine_t& engine);

        ~gpu_cmd_draw_mesh_t() override;

    public:
        void execute() override;

    private:
        void execute_resource_barrier() const;

        void execute_set_pipeline_state() const;

        void execute_set_graphic_root_signature() const;

        void execute_set_descriptor_heaps() const;

        void execute_set_graphics_root_descriptor_table() const;

        void execute_ia_set_primitive_topology() const;

        void execute_ia_set_index_buffer() const;

        void execute_ia_set_vertex_buffers() const;

        void execute_draw_indexed_instanced() const;

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
        auto get_mesh_ptr() const->std::shared_ptr<mesh_t const> const&;

        auto get_mesh_ref() const->mesh_t const&;

        void set_mesh_ptr(std::shared_ptr<mesh_t const> const& mesh);

    private:
        std::shared_ptr<mesh_t const> m_mesh{};

        // ------------------------------------------------

    public:
        auto get_instances_count() const->size_t;

        void set_instances_count(size_t const instances_count);

    private:
        size_t m_instances_count{};

        // ------------------------------------------------
    };
}
