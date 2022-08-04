#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class ECullMode : std::uint8_t
	{
		None,
		Front,
		Back,
	};

	// ----------------------------------------------------
	// DirectX

	ECullMode FromInternalEnum(D3D12_CULL_MODE const cullMode);

	D3D12_CULL_MODE ToInternalEnum(ECullMode const cullMode);

	// ----------------------------------------------------
	// String

	bool TryParse(std::string const& enumStr, ECullMode& enumVal);

	std::string ToString(ECullMode const cullMode);

	// ----------------------------------------------------
	// Json

	void from_json(nlohmann::json const& j, ECullMode& p);

	void to_json(nlohmann::json& j, ECullMode const& p);
}
