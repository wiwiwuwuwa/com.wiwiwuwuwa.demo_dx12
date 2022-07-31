#include <pch.h>
#include <aiva/layer1/e_gpu_resource_buffer_format.h>

aiva::layer1::EGpuResourceBufferFormat aiva::layer1::FromInternalEnum(DXGI_FORMAT const format)
{
	switch (format)
	{
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return EGpuResourceBufferFormat::R32G32B32A32_FLOAT;

	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return EGpuResourceBufferFormat::R8G8B8A8_UNORM;

	case DXGI_FORMAT_D32_FLOAT:
		return EGpuResourceBufferFormat::D32_FLOAT;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

DXGI_FORMAT aiva::layer1::ToInternalEnum(EGpuResourceBufferFormat const format)
{
	switch (format)
	{
	case EGpuResourceBufferFormat::R32G32B32A32_FLOAT:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;

	case EGpuResourceBufferFormat::R8G8B8A8_UNORM:
		return DXGI_FORMAT_R8G8B8A8_UNORM;

	case EGpuResourceBufferFormat::D32_FLOAT:
		return DXGI_FORMAT_D32_FLOAT;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

bool aiva::layer1::TryParse(std::string const& enumStr, EGpuResourceBufferFormat& enumVal)
{
	if (enumStr == "R32G32B32A32_FLOAT")
	{
		enumVal = EGpuResourceBufferFormat::R32G32B32A32_FLOAT;
		return true;
	}

	if (enumStr == "R8G8B8A8_UNORM")
	{
		enumVal = EGpuResourceBufferFormat::R8G8B8A8_UNORM;
		return true;
	}

	if (enumStr == "D32_FLOAT")
	{
		enumVal = EGpuResourceBufferFormat::D32_FLOAT;
		return true;
	}

	enumVal = {};
	return false;
}

std::string aiva::layer1::ToString(EGpuResourceBufferFormat const format)
{
	switch (format)
	{
	case EGpuResourceBufferFormat::R32G32B32A32_FLOAT:
		return "R32G32B32A32_FLOAT";

	case EGpuResourceBufferFormat::R8G8B8A8_UNORM:
		return "R8G8B8A8_UNORM";

	case EGpuResourceBufferFormat::D32_FLOAT:
		return "D32_FLOAT";

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

void aiva::layer1::from_json(nlohmann::json const& j, EGpuResourceBufferFormat& p)
{
	aiva::utils::Asserts::CheckBool(TryParse(j.get<std::string>(), p));
}

void aiva::layer1::to_json(nlohmann::json& j, EGpuResourceBufferFormat const& p)
{
	j = ToString(p);
}

bool aiva::layer1::SupportsDepthStencil(EGpuResourceBufferFormat const format)
{
	switch (format)
	{
	case EGpuResourceBufferFormat::D32_FLOAT:
		return true;

	default:
		return false;
	}
}
