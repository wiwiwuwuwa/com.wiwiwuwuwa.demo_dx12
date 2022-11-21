#pragma once
#include <pch.h>

#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	enum class EResourceBufferFormat : std::uint8_t
	{
		UNKNOWN,
		R32G32B32A32_FLOAT,
		R8G8B8A8_UNORM,
		D32_FLOAT,
	};

	// ----------------------------------------------------
	// DirectX

	EResourceBufferFormat FromInternalEnum(DXGI_FORMAT const format);

	DXGI_FORMAT ToInternalEnum(EResourceBufferFormat const format);

	// ----------------------------------------------------
	// Png

	EResourceBufferFormat FromPngChannels(std::size_t const channels);

	std::size_t ToPngChannels(EResourceBufferFormat const format);

	// ----------------------------------------------------
	// String

	bool TryParse(std::string const& enumStr, EResourceBufferFormat& enumVal);

	std::string ToString(EResourceBufferFormat const format);

	// ----------------------------------------------------
	// Json

	void from_json(nlohmann::json const& j, EResourceBufferFormat& p);

	void to_json(nlohmann::json& j, EResourceBufferFormat const& p);

	// ----------------------------------------------------
	// Utils

	bool SupportsDepthStencil(EResourceBufferFormat const format);
}
