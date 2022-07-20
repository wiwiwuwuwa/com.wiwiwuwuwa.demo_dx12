#include <pch.h>
#include <aiva/layer1/e_gpu_resource_buffer_format.h>

aiva::layer1::EGpuResourceBufferFormat aiva::layer1::FromInternalEnum(DXGI_FORMAT const format)
{
	switch (format)
	{
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return EGpuResourceBufferFormat::R32G32B32A32_FLOAT;

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

	case EGpuResourceBufferFormat::D32_FLOAT:
		return DXGI_FORMAT_D32_FLOAT;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
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
