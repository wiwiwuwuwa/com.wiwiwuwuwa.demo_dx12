#include <pch.h>
#include <aiva/layer1/e_gpu_fill_mode.h>

#include <aiva/utils/asserts.h>

aiva::layer1::EGpuFillMode aiva::layer1::FromInternalEnum(D3D12_FILL_MODE const fillMode)
{
	switch (fillMode)
	{
	case D3D12_FILL_MODE_WIREFRAME:
		return EGpuFillMode::Wireframe;

	case D3D12_FILL_MODE_SOLID:
		return EGpuFillMode::Solid;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D12_FILL_MODE aiva::layer1::ToInternalEnum(EGpuFillMode const fillMode)
{
	switch (fillMode)
	{
	case EGpuFillMode::Wireframe:
		return D3D12_FILL_MODE_WIREFRAME;

	case EGpuFillMode::Solid:
		return D3D12_FILL_MODE_SOLID;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

bool aiva::layer1::TryParse(std::string const& enumStr, EGpuFillMode& enumVal)
{
	if (enumStr == "Wireframe")
	{
		enumVal = EGpuFillMode::Wireframe;
		return true;
	}

	if (enumStr == "Solid")
	{
		enumVal = EGpuFillMode::Solid;
		return true;
	}

	enumVal = {};
	return false;
}

std::string aiva::layer1::ToString(EGpuFillMode const fillMode)
{
	switch (fillMode)
	{
	case EGpuFillMode::Wireframe:
		return "Wireframe";

	case EGpuFillMode::Solid:
		return "Solid";

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

void aiva::layer1::from_json(nlohmann::json const& j, EGpuFillMode& p)
{
	aiva::utils::Asserts::CheckBool(TryParse(j.get<std::string>(), p));
}

void aiva::layer1::to_json(nlohmann::json& j, EGpuFillMode const& p)
{
	j = ToString(p);
}
