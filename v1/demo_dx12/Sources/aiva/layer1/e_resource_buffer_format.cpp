#include <pch.h>
#include <aiva/layer1/e_resource_buffer_format.h>

aiva::layer1::EResourceBufferFormat aiva::layer1::FromInternalEnum(DXGI_FORMAT const format)
{
	switch (format)
	{
	case DXGI_FORMAT_UNKNOWN:
		return EResourceBufferFormat::UNKNOWN;

	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return EResourceBufferFormat::R32G32B32A32_FLOAT;

	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return EResourceBufferFormat::R8G8B8A8_UNORM;

	case DXGI_FORMAT_D32_FLOAT:
		return EResourceBufferFormat::D32_FLOAT;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

DXGI_FORMAT aiva::layer1::ToInternalEnum(EResourceBufferFormat const format)
{
	switch (format)
	{
	case EResourceBufferFormat::UNKNOWN:
		return DXGI_FORMAT_UNKNOWN;

	case EResourceBufferFormat::R32G32B32A32_FLOAT:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;

	case EResourceBufferFormat::R8G8B8A8_UNORM:
		return DXGI_FORMAT_R8G8B8A8_UNORM;

	case EResourceBufferFormat::D32_FLOAT:
		return DXGI_FORMAT_D32_FLOAT;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

aiva::layer1::EResourceBufferFormat aiva::layer1::FromPngChannels(std::size_t const channels)
{
	switch (channels)
	{
	case 4:
		return EResourceBufferFormat::R8G8B8A8_UNORM;

	default:
		aiva::utils::Asserts::CheckBool(false, "Failed to convert png channels to resource buffer format");
		return {};
	}
}

std::size_t aiva::layer1::ToPngChannels(EResourceBufferFormat const format)
{
	switch (format)
	{
	case EResourceBufferFormat::R8G8B8A8_UNORM:
		return 4;

	default:
		aiva::utils::Asserts::CheckBool(false, "Failed to convert resource buffer format to png channels");
		return {};
	}
}

bool aiva::layer1::TryParse(std::string const& enumStr, EResourceBufferFormat& enumVal)
{
	if (enumStr == "UNKNOWN")
	{
		enumVal = EResourceBufferFormat::UNKNOWN;
		return true;
	}

	if (enumStr == "R32G32B32A32_FLOAT")
	{
		enumVal = EResourceBufferFormat::R32G32B32A32_FLOAT;
		return true;
	}

	if (enumStr == "R8G8B8A8_UNORM")
	{
		enumVal = EResourceBufferFormat::R8G8B8A8_UNORM;
		return true;
	}

	if (enumStr == "D32_FLOAT")
	{
		enumVal = EResourceBufferFormat::D32_FLOAT;
		return true;
	}

	enumVal = {};
	return false;
}

std::string aiva::layer1::ToString(EResourceBufferFormat const format)
{
	switch (format)
	{
	case EResourceBufferFormat::UNKNOWN:
		return "UNKNOWN";

	case EResourceBufferFormat::R32G32B32A32_FLOAT:
		return "R32G32B32A32_FLOAT";

	case EResourceBufferFormat::R8G8B8A8_UNORM:
		return "R8G8B8A8_UNORM";

	case EResourceBufferFormat::D32_FLOAT:
		return "D32_FLOAT";

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

void aiva::layer1::from_json(nlohmann::json const& j, EResourceBufferFormat& p)
{
	aiva::utils::Asserts::CheckBool(TryParse(j.get<std::string>(), p));
}

void aiva::layer1::to_json(nlohmann::json& j, EResourceBufferFormat const& p)
{
	j = ToString(p);
}

bool aiva::layer1::SupportsDepthStencil(EResourceBufferFormat const format)
{
	switch (format)
	{
	case EResourceBufferFormat::D32_FLOAT:
		return true;

	default:
		return false;
	}
}
