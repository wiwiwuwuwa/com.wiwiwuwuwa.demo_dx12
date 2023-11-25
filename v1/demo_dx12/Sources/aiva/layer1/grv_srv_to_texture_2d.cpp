#include <pch.h>
#include <aiva/layer1/grv_srv_to_texture_2d.h>

#include <aiva/layer1/a_graphic_resource.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_texture_2d.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/object_utils.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	GrvSrvToTexture2D::GrvSrvToTexture2D(EngineType const& engine) : AGraphicResourceView{ engine }
	{

	}

	GrvSrvToTexture2D::~GrvSrvToTexture2D()
	{

	}

	GrvSrvToTexture2D::HeapTypeEnum GrvSrvToTexture2D::HeapType() const
	{
		return HeapTypeEnum::CbvSrvUav;
	}

	GrvSrvToTexture2D::ViewTypeEnum GrvSrvToTexture2D::ViewType() const
	{
		return ViewTypeEnum::Srv;
	}

	GrvSrvToTexture2D::ParentType::ResourceTypeShared GrvSrvToTexture2D::CreateDefaultInternalResource() const
	{
		auto const resource = NewObject<ResourceType>(Engine());
		Asserts::CheckBool(resource, "Resource is not valid");

		resource->Format(EResourceBufferFormat::R32G32B32A32_FLOAT);
		return resource;
	}

	void GrvSrvToTexture2D::CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination)
	{
		auto const& device = Engine().GraphicHardware().Device();
		winrt::check_bool(device);

		auto const texture2D = std::dynamic_pointer_cast<ResourceType>(GetInternalResource());
		Asserts::CheckBool(texture2D, "Texture 2D is not valid");

		auto const& resource = texture2D->GetInternalResource();
		winrt::check_bool(resource);

		auto const resourceDesc = resource->GetDesc();

		auto viewDesc = D3D12_SHADER_RESOURCE_VIEW_DESC{};
		viewDesc.Format = resourceDesc.Format;
		viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		viewDesc.Texture2D.MostDetailedMip = 0;
		viewDesc.Texture2D.MipLevels = -1;
		viewDesc.Texture2D.PlaneSlice = 0;
		viewDesc.Texture2D.ResourceMinLODClamp = 0.0f;

		device->CreateShaderResourceView(resource.get(), &viewDesc, destination);
	}

	std::vector<D3D12_RESOURCE_BARRIER> GrvSrvToTexture2D::CreateDirectxBarriers(bool const active)
	{
		auto const& resource = GetInternalResource();
		Asserts::CheckBool(resource, "Resource is not valid");

		auto const state = active ? D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_COMMON;
		return resource->CreateDirectxBarriers(state);
	}
}
