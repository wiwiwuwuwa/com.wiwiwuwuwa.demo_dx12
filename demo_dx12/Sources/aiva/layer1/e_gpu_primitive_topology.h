#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EGpuPrimitiveTopology : std::uint8_t
	{
		Undefined,
		TriangleList,
	};

	EGpuPrimitiveTopology FromInternalEnum(D3D_PRIMITIVE_TOPOLOGY const primitiveTopology);

	D3D_PRIMITIVE_TOPOLOGY ToInternalEnum(EGpuPrimitiveTopology const primitiveTopology);
}
