#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/ro_material_gr_fwd.h>
#include <aiva/layer1_ext/ro_shader_frag_fwd.h>
#include <aiva/layer1_ext/ro_shader_vert_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_code_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_register_fwd.h>
#include <aiva/layer1_ext/shader_rs_root_descriptor_table_fwd.h>
#include <aiva/layer1_ext/shader_rs_root_signature_desc_fwd.h>
#include <aiva/layer1_ext/shader_struct_register_info_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct ShaderRSRootSignatureDesc final : public aiva::utils_ext::TObject<ShaderRSRootSignatureDesc>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ShaderRSRootSignatureDesc);

	protected:
		ShaderRSRootSignatureDesc(std::shared_ptr<RoMaterialGr> const& graphicMaterial, aiva::layer1::Engine& engine);

		ShaderRSRootSignatureDesc(std::vector<ShaderStructRegisterInfo> const& registersInfo, aiva::layer1::Engine& engine);

		ShaderRSRootSignatureDesc(std::vector<std::shared_ptr<ShaderMetaForRegister>> const& registersMeta, aiva::layer1::Engine& engine);

	public:
		~ShaderRSRootSignatureDesc() override;

	private:
		static std::vector<ShaderStructRegisterInfo> ExtractRegisters(std::shared_ptr<RoMaterialGr> const& graphicMaterial);

		static std::vector<ShaderStructRegisterInfo> ExtractRegisters(std::shared_ptr<RoShaderFrag> const& fragShader);

		static std::vector<ShaderStructRegisterInfo> ExtractRegisters(std::shared_ptr<RoShaderVert> const& vertShader);

		static std::vector<ShaderStructRegisterInfo> ExtractRegisters(std::shared_ptr<ShaderMetaForCode> const& codeMeta);

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_4(public, private, D3D12_VERSIONED_ROOT_SIGNATURE_DESC, NativeResource);

		M_OBJECT_FIELD_VAR_3(private, std::vector<D3D12_ROOT_PARAMETER1>, ShadowForElementsData);

		M_OBJECT_FIELD_VAR_3(private, std::vector<std::shared_ptr<ShaderRSRootDescriptorTable>>, ShadowForElementsView);
	};
}
