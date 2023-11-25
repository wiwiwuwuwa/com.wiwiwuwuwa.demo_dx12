#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	D3D12_CPU_DESCRIPTOR_HANDLE operator+(D3D12_CPU_DESCRIPTOR_HANDLE const& handle, size_t const offset);
}
