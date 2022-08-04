#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EFillMode : std::uint8_t
	{
		Wireframe,
		Solid,
	};

	EFillMode FromInternalEnum(D3D12_FILL_MODE const fillMode);

	D3D12_FILL_MODE ToInternalEnum(EFillMode const fillMode);

	bool TryParse(std::string const& enumStr, EFillMode& enumVal);

	std::string ToString(EFillMode const fillMode);

	void from_json(nlohmann::json const& j, EFillMode& p);

	void to_json(nlohmann::json& j, EFillMode const& p);
}
