#include <pch.h>
#include <aiva/layer1/e_cull_mode.h>

#include <aiva/utils/asserts.h>

aiva::layer1::ECullMode aiva::layer1::FromInternalEnum(D3D12_CULL_MODE const cullMode)
{
	switch (cullMode)
	{
	case D3D12_CULL_MODE_NONE:
		return ECullMode::None;

	case D3D12_CULL_MODE_FRONT:
		return ECullMode::Front;

	case D3D12_CULL_MODE_BACK:
		return ECullMode::Back;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D12_CULL_MODE aiva::layer1::ToInternalEnum(ECullMode const cullMode)
{
	switch (cullMode)
	{
	case ECullMode::None:
		return D3D12_CULL_MODE_NONE;

	case ECullMode::Front:
		return D3D12_CULL_MODE_FRONT;

	case ECullMode::Back:
		return D3D12_CULL_MODE_BACK;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

bool aiva::layer1::TryParse(std::string const& enumStr, ECullMode& enumVal)
{
	if (enumStr == "None")
	{
		enumVal = ECullMode::None;
		return true;
	}

	if (enumStr == "Front")
	{
		enumVal = ECullMode::Front;
		return true;
	}

	if (enumStr == "Back")
	{
		enumVal = ECullMode::Back;
		return true;
	}

	enumVal = {};
	return false;
}

std::string aiva::layer1::ToString(ECullMode const cullMode)
{
	switch (cullMode)
	{
	case ECullMode::None:
		return "None";

	case ECullMode::Front:
		return "Front";

	case ECullMode::Back:
		return "Back";

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

void aiva::layer1::from_json(nlohmann::json const& j, ECullMode& p)
{
	aiva::utils::Asserts::CheckBool(TryParse(j.get<std::string>(), p));
}

void aiva::layer1::to_json(nlohmann::json& j, ECullMode const& p)
{
	j = ToString(p);
}
