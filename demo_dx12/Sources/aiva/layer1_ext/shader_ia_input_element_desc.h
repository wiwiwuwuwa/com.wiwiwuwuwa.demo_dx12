#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/shader_ia_input_element_desc_fwd.h>
#include <aiva/layer1_ext/shader_layout_for_field_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_field_fwd.h>
#include <aiva/utils_ext/e_value_type.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct ShaderIAInputElementDesc final : public aiva::utils_ext::TObject<ShaderIAInputElementDesc>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ShaderIAInputElementDesc);

	protected:
		ShaderIAInputElementDesc(std::shared_ptr<ShaderMetaForField> const& fieldMeta, std::shared_ptr<ShaderLayoutForField> const& fieldLayout, D3D12_INPUT_ELEMENT_DESC& nativeResource, aiva::layer1::Engine& engine);

	public:
		~ShaderIAInputElementDesc() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_REF_3(public, D3D12_INPUT_ELEMENT_DESC, NativeResource);

		M_OBJECT_FIELD_VAR_3(private, std::string, ShadowForSemanticName);
	};
}
