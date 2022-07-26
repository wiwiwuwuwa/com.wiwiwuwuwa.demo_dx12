#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EGpuFillMode : std::uint8_t
	{
		Wireframe,
		Solid,
	};

	EGpuFillMode FromInternalEnum(D3D12_FILL_MODE const fillMode);

	D3D12_FILL_MODE ToInternalEnum(EGpuFillMode const fillMode);
}
