#include <pch.h>
#include <aiva/layer1_ext/rt_resource_descriptor_function.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/graphic_hardware.h>

namespace aiva::layer1_ext
{
	void RtResourceDescriptorFunction<RtResourceType::Texture2D, RtResourceViewType::Srv>(RtResourceDescriptorParams<RtResourceType::Texture2D, RtResourceViewType::Srv> const& params)
	{
		aiva::utils::Asserts::CheckBool(params.InResource(), "In resource is not valid");
		auto const resourceDesc = params.InResource()->GetDesc();

		auto viewDesc = D3D12_SHADER_RESOURCE_VIEW_DESC{};
		viewDesc.Format = resourceDesc.Format;
		viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		if (params.InParams().MipSlice().has_value())
		{
			viewDesc.Texture2D.MostDetailedMip = params.InParams().MipSlice().value();
			viewDesc.Texture2D.MipLevels = 1;
			viewDesc.Texture2D.PlaneSlice = params.InParams().PlaneSlice();
			viewDesc.Texture1D.ResourceMinLODClamp = {};
		}
		else
		{
			viewDesc.Texture2D.MostDetailedMip = {};
			viewDesc.Texture2D.MipLevels = -1;
			viewDesc.Texture2D.PlaneSlice = params.InParams().PlaneSlice();
			viewDesc.Texture1D.ResourceMinLODClamp = {};
		}

		params.RefEngine().GraphicHardwareExt().Device().CreateShaderResourceView(params.InResource().get(), &viewDesc, params.OutHandle());
	}

	void RtResourceDescriptorFunction<RtResourceType::Texture2D, RtResourceViewType::Rtv>(RtResourceDescriptorParams<RtResourceType::Texture2D, RtResourceViewType::Rtv> const& params)
	{
		aiva::utils::Asserts::CheckBool(params.InResource(), "In resource is not valid");
		auto const resourceDesc = params.InResource()->GetDesc();

		auto viewDesc = D3D12_RENDER_TARGET_VIEW_DESC{};
		viewDesc.Format = resourceDesc.Format;
		viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipSlice = static_cast<UINT>(params.InParams().MipSlice());
		viewDesc.Texture2D.PlaneSlice = static_cast<UINT>(params.InParams().PlaneSlice());

		params.RefEngine().GraphicHardwareExt().Device().CreateRenderTargetView(params.InResource().get(), &viewDesc, params.OutHandle());
	}

	void RtResourceDescriptorFunction<RtResourceType::Texture2D, RtResourceViewType::Dsv>(RtResourceDescriptorParams<RtResourceType::Texture2D, RtResourceViewType::Dsv> const& params)
	{
		aiva::utils::Asserts::CheckBool(params.InResource(), "In resource is not valid");
		auto const resourceDesc = params.InResource()->GetDesc();

		auto viewDesc = D3D12_DEPTH_STENCIL_VIEW_DESC{};
		viewDesc.Format = resourceDesc.Format;
		viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		viewDesc.Flags = D3D12_DSV_FLAG_NONE;
		viewDesc.Texture2D.MipSlice = static_cast<UINT>(params.InParams().MipSlice());

		params.RefEngine().GraphicHardwareExt().Device().CreateDepthStencilView(params.InResource().get(), &viewDesc, params.OutHandle());
	}
}
