#pragma once
#include <pch.h>

#include <aiva/layer1/e_resource_buffer_format.h>
#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/nr_texture_2d_fwd.h>
#include <aiva/layer1_ext/ro_texture_2d_fwd.h>
#include <aiva/layer1_ext/ut_resource_barriers_emitter.h>
#include <aiva/utils_ext/i_serializable_json.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_val_dirty_flags_eager.h>
#include <aiva/utils_ext/m_object_field_var_custom.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct RoTexture2D final : public aiva::utils_ext::TObject<RoTexture2D>, public aiva::utils_ext::ISerializableJson, public UtResourceBarriersEmitter
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(RoTexture2D);

	protected:
		RoTexture2D(aiva::layer1::Engine& engine);

		RoTexture2D(winrt::com_ptr<ID3D12Resource> const& resource, aiva::layer1::Engine& engine);

		RoTexture2D(std::shared_ptr<NrTexture2D> const& texture2D, aiva::layer1::Engine& engine);

	public:
		~RoTexture2D() override;

	// ----------------------------------------------------
	// Serialization

	public:
		void DeserealizeFromJson(nlohmann::json const& resourceJsn) override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER();

	private:
		void InitGenericFields();

		void ShutGenericFields();

	private:
		void GenericFields_When_NativeResourceFields_OnDirtied();

	// ----------------------------------------------------
	// Native Resource Fields

	public:
		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER_2(DirtyFlagsForNativeResource, private);

	public:
		M_OBJECT_FIELD_VAR_CUSTOM_4(std::shared_ptr<NrTexture2D>, NativeResource, public, private);

	private:
		void InitNativeResourceFields();

		void InitNativeResourceFields(winrt::com_ptr<ID3D12Resource> const& resource);

		void InitNativeResourceFields(std::shared_ptr<NrTexture2D> const& texture2D);

		void ShutNativeResourceFields();

	private:
		void NativeResourceFields_When_NativeResource_OnDirtied();

	// ----------------------------------------------------
	// Native Resource Shortcut Fields

	public:
		M_OBJECT_FIELD_VAR_CUSTOM_4(aiva::layer1::EResourceBufferFormat, Format, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(std::size_t, Width, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(std::size_t, Height, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(bool, SupportDepthStencil, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(bool, SupportMipMapping, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(bool, SupportRenderTarget, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(bool, SupportShaderAtomics, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(bool, SupportUnorderedAccess, public, public);

	// ----------------------------------------------------
	// Resource Barriers Emitter

	public:
		std::vector<D3D12_RESOURCE_BARRIER> TransiteResourceBarriers(std::optional<std::size_t> const subResource, D3D12_RESOURCE_STATES const desiredState) override;
	};
}
