#include <pch.h>
#include <aiva2/gpu_cmd_draw_mesh.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/graphic_material.hpp>
#include <aiva2/graphic_shader.hpp>
#include <aiva2/material_property_block.hpp>
#include <aiva2/mesh.hpp>

namespace aiva2
{
    gpu_cmd_draw_mesh_t::gpu_cmd_draw_mesh_t(engine_t& engine)
        : impl_type{ engine }
    {

    }

    gpu_cmd_draw_mesh_t::~gpu_cmd_draw_mesh_t()
    {

    }

    void gpu_cmd_draw_mesh_t::execute()
    {
        execute_resource_barrier();
        execute_set_pipeline_state();
        execute_set_graphic_root_signature();
        execute_set_descriptor_heaps();
        execute_set_graphics_root_descriptor_table();
        execute_ia_set_primitive_topology();
        execute_ia_set_index_buffer();
        execute_ia_set_vertex_buffers();
        execute_draw_indexed_instanced();
    }

    void gpu_cmd_draw_mesh_t::execute_resource_barrier() const
    {
        if (auto const& material = get_material_ptr())
        {
            for (auto const& resource_barrier : (*material).init_for_rendering())
            {
                get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &resource_barrier);
            }
        }

        if (auto const& property_block = get_property_block_ptr())
        {
            for (auto const& resource_barrier : (*property_block).init_for_rendering())
            {
                get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &resource_barrier);
            }
        }

        if (auto const& mesh = get_mesh_ptr())
        {
            for (auto const& resource_barrier : (*mesh).init_for_rendering())
            {
                get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &resource_barrier);
            }
        }
    }

    void gpu_cmd_draw_mesh_t::execute_set_pipeline_state() const
    {
        auto& pupeline_state = get_material_ref().get_shader_ref().get_pipeline_state_ref();
        get_engine().get_graphic_hardware().get_command_list().SetPipelineState(&pupeline_state);
    }

    void gpu_cmd_draw_mesh_t::execute_set_graphic_root_signature() const
    {
        auto& root_signature = get_material_ref().get_shader_ref().get_root_signature_ref();
        get_engine().get_graphic_hardware().get_command_list().SetGraphicsRootSignature(&root_signature);
    }

    void gpu_cmd_draw_mesh_t::execute_set_descriptor_heaps() const
    {
        auto descriptor_heaps = std::vector<ID3D12DescriptorHeap*>{};

        if (auto const& resource_heap = get_material_ref().get_resource_heap_ptr())
        {
            descriptor_heaps.push_back(resource_heap.get());
        }

        if (std::empty(descriptor_heaps))
        {
            return;
        }

        get_engine().get_graphic_hardware().get_command_list().SetDescriptorHeaps
        (
            /*NumDescriptorHeaps*/ static_cast<UINT>(std::size(descriptor_heaps)),
            /*ppDescriptorHeaps*/ std::data(descriptor_heaps)
        );
    }

    void gpu_cmd_draw_mesh_t::execute_set_graphics_root_descriptor_table() const
    {
        auto root_parameter_index = size_t{};

        if (auto const& property_block = get_property_block_ptr())
        {
            get_engine().get_graphic_hardware().get_command_list().SetGraphicsRootConstantBufferView
            (
                /*RootParameterIndex*/ static_cast<UINT>(root_parameter_index++),
                /*BufferLocation*/ (*property_block).get_gpu_virtual_address()
            );
        }

        if (auto const& resource_heap = get_material_ref().get_resource_heap_ptr())
        {
            get_engine().get_graphic_hardware().get_command_list().SetGraphicsRootDescriptorTable
            (
                /*RootParameterIndex*/ static_cast<UINT>(root_parameter_index++),
                /*BaseDescriptor*/ (*resource_heap).GetGPUDescriptorHandleForHeapStart()
            );
        }
    }

    void gpu_cmd_draw_mesh_t::execute_ia_set_primitive_topology() const
    {
        get_engine().get_graphic_hardware().get_command_list().IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    void gpu_cmd_draw_mesh_t::execute_ia_set_index_buffer() const
    {
        auto const indx_view = get_mesh_ref().bind_for_rendering_indx();
        get_engine().get_graphic_hardware().get_command_list().IASetIndexBuffer(&indx_view);
    }

    void gpu_cmd_draw_mesh_t::execute_ia_set_vertex_buffers() const
    {
        assert_t::check_bool(false, "not implemented");
    }

    void gpu_cmd_draw_mesh_t::execute_draw_indexed_instanced() const
    {
        get_engine().get_graphic_hardware().get_command_list().DrawIndexedInstanced
        (
            /*IndexCountPerInstance*/ static_cast<UINT>(get_mesh_ref().num_indx_data()),
            /*InstanceCount*/ static_cast<UINT>(get_instances_count()),
            /*StartIndexLocation*/ {},
            /*BaseVertexLocation*/ {},
            /*StartInstanceLocation*/ {}
        );
    }

    auto gpu_cmd_draw_mesh_t::get_material_ptr() const->std::shared_ptr<graphic_material_t> const&
    {
        return m_material;
    }

    auto gpu_cmd_draw_mesh_t::get_material_ref() const->graphic_material_t&
    {
        assert_t::check_bool(m_material, "(m_material) is not valid");
        return (*m_material);
    }

    void gpu_cmd_draw_mesh_t::set_material_ptr(std::shared_ptr<graphic_material_t> const& material)
    {
        m_material = material;
    }

    auto gpu_cmd_draw_mesh_t::get_property_block_ptr() const->std::shared_ptr<material_property_block_t const> const&
    {
        return m_property_block;
    }

    auto gpu_cmd_draw_mesh_t::get_property_block_ref() const->material_property_block_t const&
    {
        assert_t::check_bool(m_property_block, "(m_property_block) is not valid");
        return (*m_property_block);
    }

    void gpu_cmd_draw_mesh_t::set_property_block_ptr(std::shared_ptr<material_property_block_t const> const& property_block)
    {
        m_property_block = property_block;
    }

    auto gpu_cmd_draw_mesh_t::get_mesh_ptr() const->std::shared_ptr<mesh_t const> const&
    {
        return m_mesh;
    }

    auto gpu_cmd_draw_mesh_t::get_mesh_ref() const->mesh_t const&
    {
        assert_t::check_bool(m_mesh, "(m_mesh) is not valid");
        return (*m_mesh);
    }

    void gpu_cmd_draw_mesh_t::set_mesh_ptr(std::shared_ptr<mesh_t const> const& mesh)
    {
        m_mesh = mesh;
    }

    auto gpu_cmd_draw_mesh_t::get_instances_count() const->size_t
    {
        return m_instances_count;
    }

    void gpu_cmd_draw_mesh_t::set_instances_count(size_t const instances_count)
    {
        m_instances_count = instances_count;
    }
}
