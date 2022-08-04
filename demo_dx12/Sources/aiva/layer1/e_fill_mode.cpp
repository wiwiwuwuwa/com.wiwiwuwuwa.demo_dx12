#include <pch.h>
#include <aiva/layer1/e_fill_mode.h>

#include <aiva/utils/asserts.h>

aiva::layer1::EFillMode aiva::layer1::FromInternalEnum(D3D12_FILL_MODE const fillMode)
{
	switch (fillMode)
	{
	case D3D12_FILL_MODE_WIREFRAME:
		return EFillMode::Wireframe;

	case D3D12_FILL_MODE_SOLID:
		return EFillMode::Solid;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D12_FILL_MODE aiva::layer1::ToInternalEnum(EFillMode const fillMode)
{
	switch (fillMode)
	{
	case EFillMode::Wireframe:
		return D3D12_FILL_MODE_WIREFRAME;

	case EFillMode::Solid:
		return D3D12_FILL_MODE_SOLID;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

bool aiva::layer1::TryParse(std::string const& enumStr, EFillMode& enumVal)
{
	if (enumStr == "Wireframe")
	{
		enumVal = EFillMode::Wireframe;
		return true;
	}

	if (enumStr == "Solid")
	{
		enumVal = EFillMode::Solid;
		return true;
	}

	enumVal = {};
	return false;
}

std::string aiva::layer1::ToString(EFillMode const fillMode)
{
	switch (fillMode)
	{
	case EFillMode::Wireframe:
		return "Wireframe";

	case EFillMode::Solid:
		return "Solid";

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

void aiva::layer1::from_json(nlohmann::json const& j, EFillMode& p)
{
	aiva::utils::Asserts::CheckBool(TryParse(j.get<std::string>(), p));
}

void aiva::layer1::to_json(nlohmann::json& j, EFillMode const& p)
{
	j = ToString(p);
}
