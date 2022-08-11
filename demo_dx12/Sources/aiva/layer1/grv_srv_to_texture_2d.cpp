#include <pch.h>
#include <aiva/layer1/grv_srv_to_texture_2d.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_texture_2d.h>
#include <aiva/layer1/graphic_hardware.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	GrvSrvToTexture2D::GrvSrvToTexture2D(EngineType const& engine) : AGraphicResourceView{ engine }
	{

	}

	GrvSrvToTexture2D::~GrvSrvToTexture2D()
	{

	}

	EDescriptorHeapType GrvSrvToTexture2D::HeapType() const
	{
		return EDescriptorHeapType::CbvSrvUav;
	}

	EResourceViewType GrvSrvToTexture2D::ViewType() const
	{
		return EResourceViewType::Srv;
	}

	void GrvSrvToTexture2D::CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination)
	{
		auto const& device = Engine().GraphicHardware().Device();
		winrt::check_bool(device);

		auto const texture2D = std::dynamic_pointer_cast<GrTexture2D>(GetInternalResource());
		Asserts::CheckBool(texture2D, "Texture 2D is not valid");

		auto const& resource = texture2D->InternalResource();
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
		auto const resource = GetInternalResource();
		Asserts::CheckBool(resource, "Resource is not valid");

		auto const& state = active ? D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_COMMON;
		return resource->PrepareBarriers(state);
	}
}
