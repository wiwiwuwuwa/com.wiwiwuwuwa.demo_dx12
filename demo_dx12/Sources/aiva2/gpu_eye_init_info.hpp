#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/struct.hpp>

namespace aiva2
{
	struct gpu_eye_init_info_t final : public implements_t<gpu_eye_init_info_t, struct_t>
	{
		// ------------------------------------------------

	public:
		auto get_descriptor_heap_type() const->D3D12_DESCRIPTOR_HEAP_TYPE;
		
		auto set_descriptor_heap_type(D3D12_DESCRIPTOR_HEAP_TYPE const value) -> this_type&;

	private:
		D3D12_DESCRIPTOR_HEAP_TYPE m_descriptor_heap_type{};

		// ------------------------------------------------

	public:
		auto get_descriptor_heap_flag() const->D3D12_DESCRIPTOR_HEAP_FLAGS;

		auto set_descriptor_heap_flag(D3D12_DESCRIPTOR_HEAP_FLAGS const value) -> this_type&;
		
	private:
		D3D12_DESCRIPTOR_HEAP_FLAGS m_descriptor_heap_flag{};

		// ------------------------------------------------
	};
}
