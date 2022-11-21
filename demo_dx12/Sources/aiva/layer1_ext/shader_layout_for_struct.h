#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/shader_layout_for_field_fwd.h>
#include <aiva/layer1_ext/shader_layout_for_struct_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_field_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_struct_fwd.h>
#include <aiva/layer1_ext/shader_packing_rules_for_struct.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct ShaderLayoutForStruct final : public aiva::utils_ext::TObject<ShaderLayoutForStruct>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ShaderLayoutForStruct);

	protected:
		ShaderLayoutForStruct(std::shared_ptr<ShaderMetaForStruct> const& metaForStruct, std::size_t const offset, ShaderPackingRulesForStruct const rules, aiva::layer1::Engine& engine);

	public:
		~ShaderLayoutForStruct() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, Offset);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, Size);

		M_OBJECT_FIELD_VAR_4(public, private, std::vector<std::shared_ptr<ShaderLayoutForField>>, Fields);

	// ----------------------------------------------------
	// Packing Fields

	private:
		void InitFieldsBeforeRules(std::size_t const offset);

		void InitForTightPackingRules(std::vector<std::shared_ptr<ShaderMetaForField>> const metaForFields);
	};
}
