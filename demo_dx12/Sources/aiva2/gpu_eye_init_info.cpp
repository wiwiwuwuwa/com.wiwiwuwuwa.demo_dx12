#include <pch.h>
#include <aiva2/gpu_eye_init_info.hpp>

namespace aiva2
{
	auto gpu_eye_init_info_t::get_descriptor_heap_type() const->D3D12_DESCRIPTOR_HEAP_TYPE
	{
		return m_descriptor_heap_type;
	}

	auto gpu_eye_init_info_t::set_descriptor_heap_type(D3D12_DESCRIPTOR_HEAP_TYPE const value) -> this_type&
	{
		m_descriptor_heap_type = value;
		return (*this);
	}

	auto gpu_eye_init_info_t::get_descriptor_heap_flag() const->D3D12_DESCRIPTOR_HEAP_FLAGS
	{
		return m_descriptor_heap_flag;
	}

	auto gpu_eye_init_info_t::set_descriptor_heap_flag(D3D12_DESCRIPTOR_HEAP_FLAGS const value) -> this_type&
	{
		m_descriptor_heap_flag = value;
		return (*this);
	}
}
