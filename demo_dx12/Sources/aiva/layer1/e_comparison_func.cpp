#include <pch.h>
#include <aiva/layer1/e_comparison_func.h>

#include <aiva/utils/asserts.h>

aiva::layer1::EComparisonFunc aiva::layer1::FromInternalEnum(D3D12_COMPARISON_FUNC const comparisonFunc)
{
	switch (comparisonFunc)
	{
	case D3D12_COMPARISON_FUNC_NEVER:
		return EComparisonFunc::Never;

	case D3D12_COMPARISON_FUNC_LESS:
		return EComparisonFunc::Less;

	case D3D12_COMPARISON_FUNC_EQUAL:
		return EComparisonFunc::Equal;

	case D3D12_COMPARISON_FUNC_LESS_EQUAL:
		return EComparisonFunc::LessEqual;

	case D3D12_COMPARISON_FUNC_GREATER:
		return EComparisonFunc::Greater;

	case D3D12_COMPARISON_FUNC_NOT_EQUAL:
		return EComparisonFunc::NotEqual;

	case D3D12_COMPARISON_FUNC_GREATER_EQUAL:
		return EComparisonFunc::GreaterEqual;

	case D3D12_COMPARISON_FUNC_ALWAYS:
		return EComparisonFunc::Always;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D12_COMPARISON_FUNC aiva::layer1::ToInternalEnum(EComparisonFunc const comparisonFunc)
{
	switch (comparisonFunc)
	{
	case EComparisonFunc::Never:
		return D3D12_COMPARISON_FUNC_NEVER;

	case EComparisonFunc::Less:
		return D3D12_COMPARISON_FUNC_LESS;

	case EComparisonFunc::Equal:
		return D3D12_COMPARISON_FUNC_EQUAL;

	case EComparisonFunc::LessEqual:
		return D3D12_COMPARISON_FUNC_LESS_EQUAL;

	case EComparisonFunc::Greater:
		return D3D12_COMPARISON_FUNC_GREATER;

	case EComparisonFunc::NotEqual:
		return D3D12_COMPARISON_FUNC_NOT_EQUAL;

	case EComparisonFunc::GreaterEqual:
		return D3D12_COMPARISON_FUNC_GREATER_EQUAL;

	case EComparisonFunc::Always:
		return D3D12_COMPARISON_FUNC_ALWAYS;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

bool aiva::layer1::TryParse(std::string const& enumStr, EComparisonFunc& enumVal)
{
	if (enumStr == "Never")
	{
		enumVal = EComparisonFunc::Never;
		return true;
	}

	if (enumStr == "Less")
	{
		enumVal = EComparisonFunc::Less;
		return true;
	}

	if (enumStr == "Equal")
	{
		enumVal = EComparisonFunc::Equal;
		return true;
	}

	if (enumStr == "LessEqual")
	{
		enumVal = EComparisonFunc::LessEqual;
		return true;
	}

	if (enumStr == "Greater")
	{
		enumVal = EComparisonFunc::Greater;
		return true;
	}

	if (enumStr == "NotEqual")
	{
		enumVal = EComparisonFunc::NotEqual;
		return true;
	}

	if (enumStr == "GreaterEqual")
	{
		enumVal = EComparisonFunc::GreaterEqual;
		return true;
	}

	if (enumStr == "Always")
	{
		enumVal =  EComparisonFunc::Always;
		return true;
	}

	enumVal = {};
	return false;
}

std::string aiva::layer1::ToString(EComparisonFunc const comparisonFunc)
{
	switch (comparisonFunc)
	{
	case EComparisonFunc::Never:
		return "Never";

	case EComparisonFunc::Less:
		return "Less";

	case EComparisonFunc::Equal:
		return "Equal";

	case EComparisonFunc::LessEqual:
		return "LessEqual";

	case EComparisonFunc::Greater:
		return "Greater";

	case EComparisonFunc::NotEqual:
		return "NotEqual";

	case EComparisonFunc::GreaterEqual:
		return "GreaterEqual";

	case EComparisonFunc::Always:
		return "Always";

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

void aiva::layer1::from_json(nlohmann::json const& j, EComparisonFunc& p)
{
	aiva::utils::Asserts::CheckBool(TryParse(j.get<std::string>(), p));
}

void aiva::layer1::to_json(nlohmann::json& j, EComparisonFunc const& p)
{
	j = ToString(p);
}
