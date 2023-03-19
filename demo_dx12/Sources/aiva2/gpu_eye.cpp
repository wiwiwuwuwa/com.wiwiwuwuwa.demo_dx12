#include <pch.h>
#include <aiva2/gpu_eye.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_eye_init_info.hpp>
#include <aiva2/graphic_hardware.hpp>

namespace aiva2
{
	gpu_eye_t::gpu_eye_t(engine_t& engine, init_info_type const& init_info)
		: impl_type{ engine }
	{
		init_descriptor_heap(init_info);
	}

	gpu_eye_t::~gpu_eye_t()
	{
		shut_descriptor_heap();
	}

	auto gpu_eye_t::get_cpu_descriptor_handle() const->D3D12_CPU_DESCRIPTOR_HANDLE
	{
		assert_t::check_bool(m_descriptor_heap, "m_descriptor_heap is not valid");
		return (*m_descriptor_heap).GetCPUDescriptorHandleForHeapStart();
	}
	
	auto gpu_eye_t::get_gpu_descriptor_handle() const->D3D12_GPU_DESCRIPTOR_HANDLE
	{
		assert_t::check_bool(m_descriptor_heap, "m_descriptor_heap is not valid");
		return (*m_descriptor_heap).GetGPUDescriptorHandleForHeapStart();
	}

	void gpu_eye_t::init_descriptor_heap(init_info_type const& init_info)
	{
		auto heap_desc = D3D12_DESCRIPTOR_HEAP_DESC{};
		heap_desc.Type = init_info.get_descriptor_heap_type();
		heap_desc.NumDescriptors = 1;
		heap_desc.Flags = init_info.get_descriptor_heap_flag();
		heap_desc.NodeMask = {};

		assert_t::check_hresult(get_engine().get_graphic_hardware().get_device().CreateDescriptorHeap
		(
			/*pDescriptorHeapDesc*/ &heap_desc,
			/*Heap*/ IID_PPV_ARGS(&m_descriptor_heap)
		));
		assert_t::check_bool(m_descriptor_heap, "m_descriptor_heap is not valid");
	}

	void gpu_eye_t::shut_descriptor_heap()
	{
		m_descriptor_heap = {};
	}
}
