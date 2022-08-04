#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EPrimitiveTopology : std::uint8_t
	{
		Undefined,
		TriangleList,
	};

	EPrimitiveTopology FromInternalEnum(D3D_PRIMITIVE_TOPOLOGY const primitiveTopology);

	D3D_PRIMITIVE_TOPOLOGY ToInternalEnum(EPrimitiveTopology const primitiveTopology);
}
