#pragma once
#include <pch.h>

#include <aiva/layer1_ext/e_shader_register_type.h>
#include <aiva/layer1_ext/shader_meta_for_register_fwd.h>
#include <aiva/layer1_ext/shader_struct_register_info_fwd.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct ShaderStructRegisterInfo final
	{
	// ----------------------------------------------------
	// Main

	public:
		ShaderStructRegisterInfo();

		ShaderStructRegisterInfo(std::shared_ptr<ShaderMetaForRegister> const& registerMeta);

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_VAR_4(public, private, EShaderRegisterType, Value);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, Index);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, Space);

	public:
		bool operator<(ShaderStructRegisterInfo const& other) const;

		static std::vector<ShaderStructRegisterInfo> Convert(std::vector<std::shared_ptr<ShaderMetaForRegister>> const& registersMeta);
	};
}
