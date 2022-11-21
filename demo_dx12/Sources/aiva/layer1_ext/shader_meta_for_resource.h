#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_register_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_resource_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct ShaderMetaForResource final : public aiva::utils_ext::TObject<ShaderMetaForResource>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ShaderMetaForResource);

	protected:
		ShaderMetaForResource(std::string const& text, aiva::layer1::Engine& engine);

	public:
		~ShaderMetaForResource() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_4(public, private, std::string, Name);

		M_OBJECT_FIELD_VAR_4(public, private, std::shared_ptr<ShaderMetaForRegister>, Register);
	};
}
