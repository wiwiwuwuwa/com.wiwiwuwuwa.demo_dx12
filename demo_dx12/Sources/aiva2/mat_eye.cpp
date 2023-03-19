#include <pch.h>
#include <aiva2/mat_eye.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>

namespace aiva2
{
	mat_eye_t::mat_eye_t(engine_t& engine)
		: impl_type{ engine }
	{
		init_descriptor_heap();
	}

	mat_eye_t::~mat_eye_t()
	{
		shut_descriptor_heap();
	}

	auto mat_eye_t::get_cpu_descriptor_handle() const->D3D12_CPU_DESCRIPTOR_HANDLE
	{
		return get_descriptor_heap_ref().GetCPUDescriptorHandleForHeapStart();
	}

	auto mat_eye_t::get_gpu_descriptor_handle() const->D3D12_GPU_DESCRIPTOR_HANDLE
	{
		return get_descriptor_heap_ref().GetGPUDescriptorHandleForHeapStart();
	}

	auto mat_eye_t::get_descriptor_heap_ptr() const->winrt::com_ptr<ID3D12DescriptorHeap> const&
	{
		return m_descriptor_heap;
	}

	auto mat_eye_t::get_descriptor_heap_ref() const->ID3D12DescriptorHeap&
	{
		assert_t::check_bool(m_descriptor_heap, "m_descriptor_heap is not valid");
		return (*m_descriptor_heap);
	}

	void mat_eye_t::init_descriptor_heap()
	{
		auto heap_desc = D3D12_DESCRIPTOR_HEAP_DESC{};
		heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heap_desc.NumDescriptors = 1;
		heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heap_desc.NodeMask = {};

		assert_t::check_hresult(get_engine().get_graphic_hardware().get_device().CreateDescriptorHeap
		(
			/*pDescriptorHeapDesc*/ &heap_desc,
			/*Heap*/ IID_PPV_ARGS(&m_descriptor_heap)
		));
		assert_t::check_bool(m_descriptor_heap, "m_descriptor_heap is not valid");
	}

	void mat_eye_t::shut_descriptor_heap()
	{
		m_descriptor_heap = {};
	}
}
