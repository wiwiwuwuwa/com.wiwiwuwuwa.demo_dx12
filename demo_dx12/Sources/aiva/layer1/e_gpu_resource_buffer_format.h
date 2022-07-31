#pragma once
#include <pch.h>

#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	enum class EGpuResourceBufferFormat : std::uint8_t
	{
		R32G32B32A32_FLOAT,
		R8G8B8A8_UNORM,
		D32_FLOAT,
	};

	EGpuResourceBufferFormat FromInternalEnum(DXGI_FORMAT const format);

	DXGI_FORMAT ToInternalEnum(EGpuResourceBufferFormat const format);

	bool TryParse(std::string const& enumStr, EGpuResourceBufferFormat& enumVal);

	std::string ToString(EGpuResourceBufferFormat const format);

	void from_json(nlohmann::json const& j, EGpuResourceBufferFormat& p);

	void to_json(nlohmann::json& j, EGpuResourceBufferFormat const& p);

	bool SupportsDepthStencil(EGpuResourceBufferFormat const format);
}
