#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/e_shader_register_type.h>
#include <aiva/layer1_ext/shader_meta_for_register_fwd.h>
#include <aiva/layer1_ext/shader_struct_register_info_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct ShaderMetaForRegister final : public aiva::utils_ext::TObject<ShaderMetaForRegister>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ShaderMetaForRegister);

	protected:
		ShaderMetaForRegister(std::string const& text, aiva::layer1::Engine& engine);

		ShaderMetaForRegister(ShaderStructRegisterInfo const& registerInfo, aiva::layer1::Engine& engine);

	public:
		~ShaderMetaForRegister();

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_4(public, private, EShaderRegisterType, Value);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, Index);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, Space);

	public:
		static std::vector<std::shared_ptr<ShaderMetaForRegister>> Convert(std::vector<ShaderStructRegisterInfo> const& registersInfo, aiva::layer1::Engine& engine);
	};
}
