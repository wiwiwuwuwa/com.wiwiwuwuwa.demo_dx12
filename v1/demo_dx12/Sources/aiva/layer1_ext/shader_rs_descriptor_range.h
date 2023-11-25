#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_register_fwd.h>
#include <aiva/layer1_ext/shader_rs_descriptor_range.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct ShaderRSDescriptorRange final : public aiva::utils_ext::TObject<ShaderRSDescriptorRange>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ShaderRSDescriptorRange);

	protected:
		ShaderRSDescriptorRange(std::shared_ptr<ShaderMetaForRegister> const& registerMeta, D3D12_DESCRIPTOR_RANGE1& nativeResource, aiva::layer1::Engine& engine);

	public:
		~ShaderRSDescriptorRange() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_REF_3(public, D3D12_DESCRIPTOR_RANGE1, NativeResource);
	};
}
