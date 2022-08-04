#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EComparisonFunc : std::uint8_t
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

	// ----------------------------------------------------
	// DirectX

	EComparisonFunc FromInternalEnum(D3D12_COMPARISON_FUNC const comparisonFunc);

	D3D12_COMPARISON_FUNC ToInternalEnum(EComparisonFunc const comparisonFunc);

	// ----------------------------------------------------
	// String

	bool TryParse(std::string const& enumStr, EComparisonFunc& enumVal);

	std::string ToString(EComparisonFunc const comparisonFunc);

	// ----------------------------------------------------
	// Json

	void from_json(nlohmann::json const& j, EComparisonFunc& p);

	void to_json(nlohmann::json& j, EComparisonFunc const& p);
}
