#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EGpuComparisonFunc : std::uint8_t
	{
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always,
	};

	EGpuComparisonFunc FromInternalEnum(D3D12_COMPARISON_FUNC const comparisonFunc);

	D3D12_COMPARISON_FUNC ToInternalEnum(EGpuComparisonFunc const comparisonFunc);

	bool TryParse(std::string const& enumStr, EGpuComparisonFunc& enumVal);

	std::string ToString(EGpuComparisonFunc const comparisonFunc);

	void from_json(nlohmann::json const& j, EGpuComparisonFunc& p);

	void to_json(nlohmann::json& j, EGpuComparisonFunc const& p);
}
