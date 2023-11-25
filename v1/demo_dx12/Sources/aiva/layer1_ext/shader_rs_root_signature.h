#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/ro_material_gr_fwd.h>
#include <aiva/layer1_ext/shader_rs_root_signature_fwd.h>
#include <aiva/layer1_ext/shader_rs_root_signature_desc_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct ShaderRSRootSignature final : public aiva::utils_ext::TObject<ShaderRSRootSignature>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ShaderRSRootSignature);

	protected:
		ShaderRSRootSignature(std::shared_ptr<RoMaterialGr> const& graphicMaterial, aiva::layer1::Engine& engine);

		ShaderRSRootSignature(std::shared_ptr<ShaderRSRootSignatureDesc> const& rootSignatureDesc, aiva::layer1::Engine& engine);

	public:
		~ShaderRSRootSignature() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_4(public, private, winrt::com_ptr<ID3D12RootSignature>, NativeResource);
	};
}
