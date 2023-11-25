#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_code_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_resource_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_struct_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct ShaderMetaForCode final : public aiva::utils_ext::TObject<ShaderMetaForCode>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ShaderMetaForCode);

	protected:
		ShaderMetaForCode(std::string const& text, aiva::layer1::Engine& engine);

	public:
		~ShaderMetaForCode() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);
		
		M_OBJECT_FIELD_VAR_4(public, private, std::vector<std::shared_ptr<ShaderMetaForStruct>>, Structures);

		M_OBJECT_FIELD_VAR_4(public, private, std::shared_ptr<ShaderMetaForStruct>, StructureForInput);

		M_OBJECT_FIELD_VAR_4(public, private, std::shared_ptr<ShaderMetaForStruct>, StructureForOutput);

		M_OBJECT_FIELD_VAR_4(public, private, std::vector<std::shared_ptr<ShaderMetaForResource>>, Resources);
	};
}
