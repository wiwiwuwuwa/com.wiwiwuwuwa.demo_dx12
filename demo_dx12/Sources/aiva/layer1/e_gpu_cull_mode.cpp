#include <pch.h>
#include <aiva/layer1/e_gpu_cull_mode.h>

#include <aiva/utils/asserts.h>

aiva::layer1::EGpuCullMode aiva::layer1::FromInternalEnum(D3D12_CULL_MODE const cullMode)
{
	switch (cullMode)
	{
	case D3D12_CULL_MODE_NONE:
		return EGpuCullMode::None;

	case D3D12_CULL_MODE_FRONT:
		return EGpuCullMode::Front;

	case D3D12_CULL_MODE_BACK:
		return EGpuCullMode::Back;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D12_CULL_MODE aiva::layer1::ToInternalEnum(EGpuCullMode const cullMode)
{
	switch (cullMode)
	{
	case EGpuCullMode::None:
		return D3D12_CULL_MODE_NONE;

	case EGpuCullMode::Front:
		return D3D12_CULL_MODE_FRONT;

	case EGpuCullMode::Back:
		return D3D12_CULL_MODE_BACK;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

bool aiva::layer1::TryParse(std::string const& enumStr, EGpuCullMode& enumVal)
{
	if (enumStr == "None")
	{
		enumVal = EGpuCullMode::None;
		return true;
	}

	if (enumStr == "Front")
	{
		enumVal = EGpuCullMode::Front;
		return true;
	}

	if (enumStr == "Back")
	{
		enumVal = EGpuCullMode::Back;
		return true;
	}

	enumVal = {};
	return false;
}

std::string aiva::layer1::ToString(EGpuCullMode const cullMode)
{
	switch (cullMode)
	{
	case EGpuCullMode::None:
		return "None";

	case EGpuCullMode::Front:
		return "Front";

	case EGpuCullMode::Back:
		return "Back";

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

void aiva::layer1::from_json(nlohmann::json const& j, EGpuCullMode& p)
{
	aiva::utils::Asserts::CheckBool(TryParse(j.get<std::string>(), p));
}

void aiva::layer1::to_json(nlohmann::json& j, EGpuCullMode const& p)
{
	j = ToString(p);
}
