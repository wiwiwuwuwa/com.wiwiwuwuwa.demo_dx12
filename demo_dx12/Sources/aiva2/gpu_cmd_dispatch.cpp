#include <pch.h>
#include <aiva2/gpu_cmd_dispatch.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/compute_material.hpp>
#include <aiva2/compute_shader.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>

namespace aiva2
{
    gpu_cmd_dispatch_t::gpu_cmd_dispatch_t(engine_t& engine)
        : impl_type{ engine }
    {
        
    }

    gpu_cmd_dispatch_t::~gpu_cmd_dispatch_t()
    {
        
    }

    void gpu_cmd_dispatch_t::execute()
    {
        execute_resource_barrier();
        execute_set_pipeline_state();
        execute_set_compute_root_signature();
        execute_set_descriptor_heaps();
        execute_set_compute_root_descriptor_table();
        execute_dispatch();
    }

    void gpu_cmd_dispatch_t::execute_resource_barrier() const
    {
        for (auto const& resource_barrier : get_material_ref().init_for_rendering())
        {
            get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &resource_barrier);
        }
    }
    
    void gpu_cmd_dispatch_t::execute_set_pipeline_state() const
    {
        auto& pupeline_state = get_material_ref().get_shader_ref().get_pipeline_state_ref();
        get_engine().get_graphic_hardware().get_command_list().SetPipelineState(&pupeline_state);
    }

    void gpu_cmd_dispatch_t::execute_set_compute_root_signature() const
    {
        auto& root_signature = get_material_ref().get_shader_ref().get_root_signature_ref();
        get_engine().get_graphic_hardware().get_command_list().SetComputeRootSignature(&root_signature);
    }

    void gpu_cmd_dispatch_t::execute_set_descriptor_heaps() const
    {
        auto const descriptor_heaps = std::array<ID3D12DescriptorHeap*, 1>{ &get_material_ref().get_resource_heap_ref() };
        get_engine().get_graphic_hardware().get_command_list().SetDescriptorHeaps
        (
            /*NumDescriptorHeaps*/ static_cast<UINT>(std::size(descriptor_heaps)),
            /*ppDescriptorHeaps*/ std::data(descriptor_heaps)
        );
    }

    void gpu_cmd_dispatch_t::execute_set_compute_root_descriptor_table() const
    {
        auto& resource_heap = get_material_ref().get_resource_heap_ref();
        get_engine().get_graphic_hardware().get_command_list().SetComputeRootDescriptorTable(0, resource_heap.GetGPUDescriptorHandleForHeapStart());
    }

    void gpu_cmd_dispatch_t::execute_dispatch() const
    {
        auto const thread_group_count_x = static_cast<UINT>(get_thread_group_count().x);
        auto const thread_group_count_y = static_cast<UINT>(get_thread_group_count().y);
        auto const thread_group_count_z = static_cast<UINT>(get_thread_group_count().z);

        get_engine().get_graphic_hardware().get_command_list().Dispatch(thread_group_count_x, thread_group_count_y, thread_group_count_z);
    }

    auto gpu_cmd_dispatch_t::get_material_ptr() const->std::shared_ptr<compute_material_t> const&
    {
        return m_material;
    }

    auto gpu_cmd_dispatch_t::get_material_ref() const->compute_material_t&
    {
        assert_t::check_bool(get_material_ptr(), "material is not valid");
        return (*get_material_ptr());
    }

    void gpu_cmd_dispatch_t::set_material_ptr(std::shared_ptr<compute_material_t> const& material)
    {
        m_material = material;
    }

    auto gpu_cmd_dispatch_t::get_thread_group_count() const->glm::u16vec3 const&
    {
        return m_thread_group_count;
    }

    void gpu_cmd_dispatch_t::set_thread_group_count(glm::u16vec3 const& thread_group_count)
    {
        m_thread_group_count = thread_group_count;
    }
}
