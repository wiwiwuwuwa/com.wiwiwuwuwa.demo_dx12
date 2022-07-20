#pragma once
#include <pch.h>

#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	enum class EGpuResourceBufferFormat : std::uint8_t
	{
		R32G32B32A32_FLOAT,
		D32_FLOAT,
	};

	EGpuResourceBufferFormat FromInternalEnum(DXGI_FORMAT const format);

	DXGI_FORMAT ToInternalEnum(EGpuResourceBufferFormat const format);

	bool SupportsDepthStencil(EGpuResourceBufferFormat const format);
}
