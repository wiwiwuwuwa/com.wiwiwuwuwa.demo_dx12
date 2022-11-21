#include <pch.h>
#include <aiva/layer1_ext/nr_texture_2d.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	NrTexture2D::NrTexture2D(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitGenericFields();
		InitParamsFields();
		InitObjectFields();
	}

	NrTexture2D::NrTexture2D(winrt::com_ptr<ID3D12Resource> const& nativeResource, aiva::layer1::Engine& engine) : NrTexture2D{ engine }
	{
		Asserts::CheckBool(nativeResource, "Native resource is not valid");
		Object(nativeResource);
	}

	NrTexture2D::~NrTexture2D()
	{
		ShutObjectFields();
		ShutParamsFields();
		ShutGenericFields();
	}

	void NrTexture2D::InitGenericFields()
	{
		DirtyFlagsForParamsSetter().OnAfterDirtied().AttachListener(&ThisType::GenericFields_When_ParamsFields_OnDirtied, this);
		DirtyFlagsForObjectGetter().OnAfterDirtied().AttachListener(&ThisType::GenericFields_When_ObjectFields_OnDirtied, this);
	}

	void NrTexture2D::ShutGenericFields()
	{
		DirtyFlagsForObjectGetter().OnAfterDirtied().RemoveListener(&ThisType::GenericFields_When_ObjectFields_OnDirtied, this);
		DirtyFlagsForParamsSetter().OnAfterDirtied().RemoveListener(&ThisType::GenericFields_When_ParamsFields_OnDirtied, this);
	}

	void NrTexture2D::GenericFields_When_ParamsFields_OnDirtied()
	{
		DirtyFlags().Dirty();
	}

	void NrTexture2D::GenericFields_When_ObjectFields_OnDirtied()
	{
		DirtyFlags().Dirty();
	}

	aiva::layer1::EResourceBufferFormat const& NrTexture2D::Format()
	{
		DirtyFlagsForParamsGetter().Flush();
		return mFormat;
	}

	void NrTexture2D::Format(aiva::layer1::EResourceBufferFormat const& value)
	{
		if (mFormat != value)
		{
			mFormat = value;
			DirtyFlagsForParamsSetter().Dirty();
		}
	}

	std::size_t const& NrTexture2D::Width()
	{
		DirtyFlagsForParamsGetter().Flush();
		return mWidth;
	}

	void NrTexture2D::Width(std::size_t const& value)
	{
		if (mWidth != value)
		{
			mWidth = value;
			DirtyFlagsForParamsSetter().Dirty();
		}
	}

	std::size_t const& NrTexture2D::Height()
	{
		DirtyFlagsForParamsGetter().Flush();
		return mHeight;
	}

	void NrTexture2D::Height(std::size_t const& value)
	{
		if (mHeight != value)
		{
			mHeight = value;
			DirtyFlagsForParamsSetter().Dirty();
		}
	}

	bool const& NrTexture2D::SupportDepthStencil()
	{
		DirtyFlagsForParamsGetter().Flush();
		return mSupportDepthStencil;
	}

	void NrTexture2D::SupportDepthStencil(bool const& value)
	{
		if (mSupportDepthStencil != value)
		{
			mSupportDepthStencil = value;
			DirtyFlagsForParamsSetter().Dirty();
		}
	}

	bool const& NrTexture2D::SupportMipMapping()
	{
		DirtyFlagsForParamsGetter().Flush();
		return mSupportMipMapping;
	}

	void NrTexture2D::SupportMipMapping(bool const& value)
	{
		if (mSupportMipMapping != value)
		{
			mSupportMipMapping = value;
			DirtyFlagsForParamsSetter().Dirty();
		}
	}

	bool const& NrTexture2D::SupportRenderTarget()
	{
		DirtyFlagsForParamsGetter().Flush();
		return mSupportRenderTarget;
	}

	void NrTexture2D::SupportRenderTarget(bool const& value)
	{
		if (mSupportRenderTarget != value)
		{
			mSupportRenderTarget = value;
			DirtyFlagsForParamsSetter().Dirty();
		}
	}

	bool const& NrTexture2D::SupportShaderAtomics()
	{
		DirtyFlagsForParamsGetter().Flush();
		return mSupportShaderAtomics;
	}

	void NrTexture2D::SupportShaderAtomics(bool const& value)
	{
		if (mSupportShaderAtomics != value)
		{
			mSupportShaderAtomics = value;
			DirtyFlagsForParamsSetter().Dirty();
		}
	}

	bool const& NrTexture2D::SupportUnorderedAccess()
	{
		DirtyFlagsForParamsGetter().Flush();
		return mSupportUnorderedAccess;
	}

	void NrTexture2D::SupportUnorderedAccess(bool const& value)
	{
		if (mSupportUnorderedAccess != value)
		{
			mSupportUnorderedAccess = value;
			DirtyFlagsForParamsSetter().Dirty();
		}
	}

	void NrTexture2D::InitParamsFields()
	{
		DirtyFlagsForObjectSetter().OnAfterDirtied().AttachListener(&ThisType::ParamsFields_When_ObjectFields_OnDirtied, this);
		DirtyFlagsForParamsGetter().OnBeforeFlushed().AttachListener(&ThisType::ParamsFields_When_ParamsFields_OnFlushed, this);
	}

	void NrTexture2D::ShutParamsFields()
	{
		DirtyFlagsForParamsGetter().OnBeforeFlushed().RemoveListener(&ThisType::ParamsFields_When_ParamsFields_OnFlushed, this);
		DirtyFlagsForObjectSetter().OnAfterDirtied().RemoveListener(&ThisType::ParamsFields_When_ObjectFields_OnDirtied, this);
	}

	void NrTexture2D::ParamsFields_When_ObjectFields_OnDirtied()
	{
		DirtyFlagsForParamsGetter().Dirty();
	}

	void NrTexture2D::ParamsFields_When_ParamsFields_OnFlushed()
	{
		RefreshParamsFromObject();
	}

	void NrTexture2D::RefreshParamsFromObject()
	{
		Asserts::CheckBool(mObject, "Object is not valid");

		auto heapProperties = D3D12_HEAP_PROPERTIES{};
		auto heapFlags = D3D12_HEAP_FLAGS{};
		winrt::check_hresult(mObject->GetHeapProperties(&heapProperties, &heapFlags));

		mSupportShaderAtomics = heapFlags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS;

		auto const resourceDesc = mObject->GetDesc();
		Asserts::CheckBool(resourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D, "Resource is not a texture 2D");

		mFormat = aiva::layer1::FromInternalEnum(resourceDesc.Format);
		mWidth = resourceDesc.Width;
		mHeight = resourceDesc.Height;
		mSupportDepthStencil = resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		mSupportMipMapping = resourceDesc.MipLevels != 1;
		mSupportRenderTarget = resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		mSupportUnorderedAccess = resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}

	winrt::com_ptr<ID3D12Resource> const& NrTexture2D::Object()
	{
		DirtyFlagsForObjectGetter().Flush();
		return mObject;
	}

	void NrTexture2D::Object(winrt::com_ptr<ID3D12Resource> const& value)
	{
		if (mObject != value)
		{
			mObject = value;
			DirtyFlagsForObjectSetter().Dirty();
		}
	}

	void NrTexture2D::InitObjectFields()
	{
		DirtyFlagsForParamsSetter().OnAfterDirtied().AttachListener(&ThisType::ObjectFields_When_ParamsFields_OnDirtied, this);
		DirtyFlagsForObjectGetter().OnBeforeFlushed().AttachListener(&ThisType::ObjectFields_When_ObjectFields_OnFlushed, this);
	}

	void NrTexture2D::ShutObjectFields()
	{
		DirtyFlagsForObjectGetter().OnBeforeFlushed().RemoveListener(&ThisType::ObjectFields_When_ObjectFields_OnFlushed, this);
		DirtyFlagsForParamsSetter().OnAfterDirtied().RemoveListener(&ThisType::ObjectFields_When_ParamsFields_OnDirtied, this);
	}

	void NrTexture2D::ObjectFields_When_ParamsFields_OnDirtied()
	{
		DirtyFlagsForObjectGetter().Dirty();
	}

	void NrTexture2D::ObjectFields_When_ObjectFields_OnFlushed()
	{
		RefreshObjectFromParams();
	}

	void NrTexture2D::RefreshObjectFromParams()
	{
		Asserts::CheckBool(mWidth > 0, "Width is not valid");
		Asserts::CheckBool(mHeight > 0, "Height is not valid");
		Asserts::CheckBool(!mSupportDepthStencil || aiva::layer1::SupportsDepthStencil(mFormat), "Format doesn't support depth stencil");
		Asserts::CheckBool(!(mSupportMipMapping && (mWidth != mHeight)), "Non-square texture doesn't support mip maps");
		Asserts::CheckBool(!mSupportRenderTarget || !aiva::layer1::SupportsDepthStencil(mFormat), "Format doesn't support render target");
		Asserts::CheckBool(!(mSupportRenderTarget && mSupportDepthStencil), "Format doesn't support render target and depth stencil at same time");
		Asserts::CheckBool(!(mSupportDepthStencil && mSupportUnorderedAccess), "Format doesn't support depth stencil and unordered access at same time");

		auto const& device = Engine().GraphicHardware().Device();
		Asserts::CheckBool(device, "Device is not valid");

		auto heapProperties = D3D12_HEAP_PROPERTIES{};
		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask = 0;
		heapProperties.VisibleNodeMask = 0;

		auto heapFlags = D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;
		heapFlags |= mSupportShaderAtomics ? D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS : heapFlags;

		auto resourceDesc = D3D12_RESOURCE_DESC{};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resourceDesc.Alignment = 0;
		resourceDesc.Width = mWidth;
		resourceDesc.Height = mHeight;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = mSupportMipMapping ? 0 : 1;
		resourceDesc.Format = aiva::layer1::ToInternalEnum(mFormat);
		resourceDesc.SampleDesc = { 1, 0 };
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		resourceDesc.Flags |= mSupportRenderTarget ? D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET : resourceDesc.Flags;
		resourceDesc.Flags |= mSupportDepthStencil ? D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL : resourceDesc.Flags;
		resourceDesc.Flags |= mSupportUnorderedAccess ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : resourceDesc.Flags;

		auto const resourceStates = D3D12_RESOURCE_STATE_COMMON;
		mResourceBarriers = resourceStates;

		winrt::check_hresult(device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&mObject)));
		Asserts::CheckBool(mObject, "Object is not valid");
	}

	std::vector<D3D12_RESOURCE_BARRIER> NrTexture2D::TransiteResourceBarriers(std::optional<std::size_t> const subResource, D3D12_RESOURCE_STATES const desiredState)
	{
		auto resourceBarriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		auto const& object = Object();
		if (object)
		{
			auto const resourceTransitions = mResourceBarriers.Transite(subResource, desiredState);
			for (auto const& resourceTransition : resourceTransitions)
			{
				auto& resourceBarrier = resourceBarriers.emplace_back();
				resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				resourceBarrier.Transition.pResource = mObject.get();
				resourceBarrier.Transition.Subresource = static_cast<UINT>(resourceTransition.SubResource().value_or(D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES));
				resourceBarrier.Transition.StateBefore = resourceTransition.CurrentState();
				resourceBarrier.Transition.StateAfter = resourceTransition.DesiredState();
			}

			auto const isUav = static_cast<bool>(desiredState & D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
			if (isUav)
			{
				auto& resourceBarrier = resourceBarriers.emplace_back();
				resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
				resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				resourceBarrier.UAV.pResource = mObject.get();
			}
		}

		return resourceBarriers;
	}
}
