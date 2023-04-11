#include <pch.h>
#include <aiva2/gpu_cmd_draw_proc.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/graphic_material.hpp>
#include <aiva2/graphic_shader.hpp>

namespace aiva2
{
    gpu_cmd_draw_proc_t::gpu_cmd_draw_proc_t(engine_t& engine)
        : impl_type{ engine }
    {
        
    }

    gpu_cmd_draw_proc_t::~gpu_cmd_draw_proc_t()
    {
        
    }

    void gpu_cmd_draw_proc_t::execute()
    {
        execute_resource_barrier();
        execute_set_pipeline_state();
        execute_set_graphic_root_signature();
        execute_set_descriptor_heaps();
        execute_set_graphics_root_descriptor_table();
        execute_ia_set_primitive_topology();
        execute_draw_instanced();
    }

    void gpu_cmd_draw_proc_t::execute_resource_barrier() const
    {
        for (auto const& resource_barrier : get_material_ref().init_for_rendering())
        {
            get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &resource_barrier);
        }
    }

    void gpu_cmd_draw_proc_t::execute_set_pipeline_state() const
    {
        auto& pupeline_state = get_material_ref().get_shader_ref().get_pipeline_state_ref();
        get_engine().get_graphic_hardware().get_command_list().SetPipelineState(&pupeline_state);
    }

    void gpu_cmd_draw_proc_t::execute_set_graphic_root_signature() const
    {
        auto& root_signature = get_material_ref().get_shader_ref().get_root_signature_ref();
        get_engine().get_graphic_hardware().get_command_list().SetGraphicsRootSignature(&root_signature);
    }

    void gpu_cmd_draw_proc_t::execute_set_descriptor_heaps() const
    {
        auto const descriptor_heaps = std::array<ID3D12DescriptorHeap*, 1>{ &get_material_ref().get_resource_heap_ref() };
        get_engine().get_graphic_hardware().get_command_list().SetDescriptorHeaps
        (
            /*NumDescriptorHeaps*/ static_cast<UINT>(std::size(descriptor_heaps)),
            /*ppDescriptorHeaps*/ std::data(descriptor_heaps)
        );
    }

    void gpu_cmd_draw_proc_t::execute_set_graphics_root_descriptor_table() const
    {
        auto& resource_heap = get_material_ref().get_resource_heap_ref();
        get_engine().get_graphic_hardware().get_command_list().SetGraphicsRootDescriptorTable(0, resource_heap.GetGPUDescriptorHandleForHeapStart());
    }

    void gpu_cmd_draw_proc_t::execute_ia_set_primitive_topology() const
    {
        get_engine().get_graphic_hardware().get_command_list().IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    void gpu_cmd_draw_proc_t::execute_draw_instanced() const
    {
        get_engine().get_graphic_hardware().get_command_list().DrawInstanced
        (
            /*VertexCountPerInstance*/ static_cast<UINT>(get_vertices_count()),
            /*InstanceCount*/ static_cast<UINT>(get_instances_count()),
            /*StartVertexLocation*/ {},
            /*StartInstanceLocation*/ {}
        );
    }
    
    auto gpu_cmd_draw_proc_t::get_material_ptr() const->std::shared_ptr<graphic_material_t> const&
    {
        return m_material;
    }

    auto gpu_cmd_draw_proc_t::get_material_ref() const->graphic_material_t&
    {
        auto const& material = get_material_ptr();
        assert_t::check_bool(material, "(material) is not valid");

        return (*material);
    }

    void gpu_cmd_draw_proc_t::set_material_ptr(std::shared_ptr<graphic_material_t> const& material)
    {
        m_material = material;
    }

    auto gpu_cmd_draw_proc_t::get_vertices_count() const->size_t
    {
        return m_vertices_count;
    }

    void gpu_cmd_draw_proc_t::set_vertices_count(size_t const vertices_count)
    {
        m_vertices_count = vertices_count;
    }

    auto gpu_cmd_draw_proc_t::get_instances_count() const->size_t
    {
        return m_instances_count;
    }

    void gpu_cmd_draw_proc_t::set_instances_count(size_t const instances_count)
    {
        m_instances_count = instances_count;
    }
}
