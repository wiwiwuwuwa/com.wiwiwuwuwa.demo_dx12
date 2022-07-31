#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EGpuCullMode : std::uint8_t
	{
		None,
		Front,
		Back,
	};

	EGpuCullMode FromInternalEnum(D3D12_CULL_MODE const cullMode);

	D3D12_CULL_MODE ToInternalEnum(EGpuCullMode const cullMode);

	bool TryParse(std::string const& enumStr, EGpuCullMode& enumVal);

	std::string ToString(EGpuCullMode const cullMode);

	void from_json(nlohmann::json const& j, EGpuCullMode& p);

	void to_json(nlohmann::json& j, EGpuCullMode const& p);
}
