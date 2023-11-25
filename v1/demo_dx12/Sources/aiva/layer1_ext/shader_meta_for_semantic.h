#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_semantic_fwd.h>
#include <aiva/layer1_ext/shader_parsing_rules_for_semantic.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct ShaderMetaForSemantic final : public aiva::utils_ext::TObject<ShaderMetaForSemantic>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ShaderMetaForSemantic);

	protected:
		ShaderMetaForSemantic(std::string const& text, ShaderParsingRulesForSemantic const rules, aiva::layer1::Engine& engine);

	public:
		~ShaderMetaForSemantic() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_4(public, private, std::string, Value);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, Index);

		M_OBJECT_FIELD_VAR_4(public, private, bool, IsInputAssembly);

		M_OBJECT_FIELD_VAR_4(public, private, bool, IsSystemValue);

	// ----------------------------------------------------
	// Comparison Fields

	public:
		bool operator<(ShaderMetaForSemantic const& other) const;
	};
}
