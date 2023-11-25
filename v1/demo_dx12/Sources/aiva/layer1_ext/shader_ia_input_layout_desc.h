#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/ro_shader_vert_fwd.h>
#include <aiva/layer1_ext/shader_ia_input_element_desc_fwd.h>
#include <aiva/layer1_ext/shader_ia_input_layout_desc_fwd.h>
#include <aiva/layer1_ext/shader_layout_for_struct_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_code_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_struct_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct ShaderIAInputLayoutDesc final : public aiva::utils_ext::TObject<ShaderIAInputLayoutDesc>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ShaderIAInputLayoutDesc);

	protected:
		ShaderIAInputLayoutDesc(std::shared_ptr<RoShaderVert> const& vertexShader, aiva::layer1::Engine& engine);

		ShaderIAInputLayoutDesc(std::shared_ptr<ShaderMetaForCode> const& codeMeta, aiva::layer1::Engine& engine);

		ShaderIAInputLayoutDesc(std::shared_ptr<ShaderMetaForStruct> const& structMeta, aiva::layer1::Engine& engine);

	public:
		~ShaderIAInputLayoutDesc() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_4(public, private, D3D12_INPUT_LAYOUT_DESC, NativeResource);

		M_OBJECT_FIELD_VAR_3(private, std::vector<D3D12_INPUT_ELEMENT_DESC>, ShadowForElementsData);

		M_OBJECT_FIELD_VAR_3(private, std::vector<std::shared_ptr<ShaderIAInputElementDesc>>, ShadowForElementsView);

	private:
		void InitGenericFields(std::shared_ptr<RoShaderVert> const& vertexShader);

		void InitGenericFields(std::shared_ptr<ShaderMetaForCode> const& codeMeta);

		void InitGenericFields(std::shared_ptr<ShaderMetaForStruct> const& structMeta);
	};
}
