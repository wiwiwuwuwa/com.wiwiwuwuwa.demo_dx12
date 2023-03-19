#include <pch.h>
#include <aiva2/d3d12_cpu_descriptor_handle_utils.hpp>

namespace aiva2
{
	D3D12_CPU_DESCRIPTOR_HANDLE operator+(D3D12_CPU_DESCRIPTOR_HANDLE const& handle, size_t const offset)
	{
		auto result = handle;
		result.ptr += offset;
		return result;
	}
}
