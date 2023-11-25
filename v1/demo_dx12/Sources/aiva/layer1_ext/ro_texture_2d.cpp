#include <pch.h>
#include <aiva/layer1_ext/ro_texture_2d.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/nr_sampler.h>
#include <aiva/layer1_ext/nr_texture_2d.h>
#include <aiva/layer1_ext/resource_system.h>
#include <aiva/layer1_ext/ro_image_png.h>
#include <aiva/layer1_ext/ut_graphic_command_utils.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	RoTexture2D::RoTexture2D(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitGenericFields();
		InitNativeResourceFields();
	}

	RoTexture2D::RoTexture2D(winrt::com_ptr<ID3D12Resource> const& resource, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitGenericFields();
		InitNativeResourceFields(resource);
	}

	RoTexture2D::RoTexture2D(std::shared_ptr<NrTexture2D> const& resource, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitGenericFields();
		InitNativeResourceFields(resource);
	}

	RoTexture2D::~RoTexture2D()
	{
		ShutNativeResourceFields();
		ShutGenericFields();
	}

	void RoTexture2D::DeserealizeFromJson(nlohmann::json const& resourceJsn)
	{
		Asserts::CheckBool(!std::empty(resourceJsn), "Resource jsn is not valid");

		auto const imageData = Engine().ResourceSystemExt().GetResource<RoImagePng>(resourceJsn.at("image_path").get<std::filesystem::path>());
		Asserts::CheckBool(imageData, "Image data is not valid");

		Format(aiva::layer1::FromPngChannels(imageData->Channels()));
		Width(imageData->Width());
		Height(imageData->Height());
		SupportDepthStencil(aiva::layer1::SupportsDepthStencil(aiva::layer1::FromPngChannels(imageData->Channels())));

		SupportMipMapping(resourceJsn.at("support_mip_mapping").get<std::remove_reference_t<decltype(SupportMipMapping())>>());
		SupportRenderTarget(resourceJsn.at("support_render_target").get<std::remove_reference_t<decltype(SupportRenderTarget())>>());
		SupportShaderAtomics(resourceJsn.at("support_shader_atomics").get<std::remove_reference_t<decltype(SupportShaderAtomics())>>());
		SupportUnorderedAccess(resourceJsn.at("support_unordered_access").get<std::remove_reference_t<decltype(SupportUnorderedAccess())>>());

		UtGraphicCommandUtils::CopyDataToTexture2D(imageData->Span(), NativeResource(), mEngine);

		DirtyFlags().Dirty();
	}

	void RoTexture2D::InitGenericFields()
	{
		DirtyFlagsForNativeResource().OnAfterDirtied().AttachListener(&ThisType::GenericFields_When_NativeResourceFields_OnDirtied, this);
	}

	void RoTexture2D::ShutGenericFields()
	{
		DirtyFlagsForNativeResource().OnAfterDirtied().RemoveListener(&ThisType::GenericFields_When_NativeResourceFields_OnDirtied, this);
	}

	void RoTexture2D::GenericFields_When_NativeResourceFields_OnDirtied()
	{
		DirtyFlags().Dirty();
	}

	std::shared_ptr<NrTexture2D> const& RoTexture2D::NativeResource()
	{
		return mNativeResource;
	}

	void RoTexture2D::NativeResource(std::shared_ptr<NrTexture2D> const& value)
	{
		if (mNativeResource)
		{
			mNativeResource->DirtyFlags().OnAfterDirtied().RemoveListener(&ThisType::NativeResourceFields_When_NativeResource_OnDirtied, this);
		}

		mNativeResource = value;

		if (mNativeResource)
		{
			mNativeResource->DirtyFlags().OnAfterDirtied().AttachListener(&ThisType::NativeResourceFields_When_NativeResource_OnDirtied, this);
		}

		DirtyFlagsForNativeResource().Dirty();
	}

	void RoTexture2D::InitNativeResourceFields()
	{
		auto const texture2D = NewObject<NrTexture2D>(Engine());
		Asserts::CheckBool(texture2D, "Texture 2D is not valid");

		InitNativeResourceFields(texture2D);
	}

	void RoTexture2D::InitNativeResourceFields(winrt::com_ptr<ID3D12Resource> const& resource)
	{
		Asserts::CheckBool(resource, "Resource is not valid");

		auto const texture2D = NewObject<NrTexture2D>(resource, Engine());
		Asserts::CheckBool(texture2D, "Texture 2D is not valid");

		InitNativeResourceFields(texture2D);
	}

	void RoTexture2D::InitNativeResourceFields(std::shared_ptr<NrTexture2D> const& texture2D)
	{
		Asserts::CheckBool(texture2D, "Texture 2D is not valid");
		NativeResource(texture2D);
	}

	void RoTexture2D::ShutNativeResourceFields()
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		NativeResource({});
	}

	void RoTexture2D::NativeResourceFields_When_NativeResource_OnDirtied()
	{
		DirtyFlagsForNativeResource().Dirty();
	}

	aiva::layer1::EResourceBufferFormat const& RoTexture2D::Format()
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		return NativeResource()->Format();
	}

	void RoTexture2D::Format(aiva::layer1::EResourceBufferFormat const& value)
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		NativeResource()->Format(value);
	}

	std::size_t const& RoTexture2D::Width()
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		return NativeResource()->Width();
	}

	void RoTexture2D::Width(std::size_t const& value)
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		NativeResource()->Width(value);
	}

	std::size_t const& RoTexture2D::Height()
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		return NativeResource()->Height();
	}

	void RoTexture2D::Height(std::size_t const& value)
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		NativeResource()->Height(value);
	}

	bool const& RoTexture2D::SupportDepthStencil()
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		return NativeResource()->SupportDepthStencil();
	}

	void RoTexture2D::SupportDepthStencil(bool const& value)
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		NativeResource()->SupportDepthStencil(value);
	}

	bool const& RoTexture2D::SupportMipMapping()
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		return NativeResource()->SupportMipMapping();
	}

	void RoTexture2D::SupportMipMapping(bool const& value)
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		NativeResource()->SupportMipMapping(value);
	}

	bool const& RoTexture2D::SupportRenderTarget()
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		return NativeResource()->SupportRenderTarget();
	}

	void RoTexture2D::SupportRenderTarget(bool const& value)
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		NativeResource()->SupportRenderTarget(value);
	}

	bool const& RoTexture2D::SupportShaderAtomics()
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		return NativeResource()->SupportShaderAtomics();
	}

	void RoTexture2D::SupportShaderAtomics(bool const& value)
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		NativeResource()->SupportShaderAtomics(value);
	}

	bool const& RoTexture2D::SupportUnorderedAccess()
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		return NativeResource()->SupportUnorderedAccess();
	}

	void RoTexture2D::SupportUnorderedAccess(bool const& value)
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		NativeResource()->SupportUnorderedAccess(value);
	}

	std::vector<D3D12_RESOURCE_BARRIER> RoTexture2D::TransiteResourceBarriers(std::optional<std::size_t> const subResource, D3D12_RESOURCE_STATES const desiredState)
	{
		Asserts::CheckBool(NativeResource(), "Native resource is not valid");
		return NativeResource()->TransiteResourceBarriers(subResource, desiredState);
	}
}
