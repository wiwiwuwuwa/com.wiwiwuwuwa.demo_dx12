#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_register_fwd.h>
#include <aiva/layer1_ext/shader_rs_descriptor_range_fwd.h>
#include <aiva/layer1_ext/shader_rs_root_descriptor_table_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct ShaderRSRootDescriptorTable final : public aiva::utils_ext::TObject<ShaderRSRootDescriptorTable>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ShaderRSRootDescriptorTable);

	protected:
		ShaderRSRootDescriptorTable(std::vector<std::shared_ptr<ShaderMetaForRegister>> const& registersMeta, D3D12_ROOT_PARAMETER1& nativeResource, aiva::layer1::Engine& engine);

	public:
		~ShaderRSRootDescriptorTable() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_REF_3(public, D3D12_ROOT_PARAMETER1, NativeResource);

		M_OBJECT_FIELD_VAR_3(private, std::vector<D3D12_DESCRIPTOR_RANGE1>, ShadowForElementsData);

		M_OBJECT_FIELD_VAR_3(private, std::vector<std::shared_ptr<ShaderRSDescriptorRange>>, ShadowForElementsView);
	};
}
