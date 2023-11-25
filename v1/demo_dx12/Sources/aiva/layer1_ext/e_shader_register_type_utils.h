#pragma once
#include <pch.h>

#include <aiva/layer1_ext/e_shader_register_type.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct EShaderRegisterTypeUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		EShaderRegisterTypeUtils();

		~EShaderRegisterTypeUtils();

	private:
		static EShaderRegisterTypeUtils& Instance();

	// ----------------------------------------------------
	// Validation

	public:
		static bool IsValid(EShaderRegisterType const value);

	// ----------------------------------------------------
	// HLSL Parsing

	public:
		M_OBJECT_FIELD_VAR_3(private, M_SINGLE_ARG(std::unordered_map<std::string, EShaderRegisterType>), HLSLToShaderRegisterType);

	public:
		static EShaderRegisterType ParseFromHLSL(std::string const& typeName);

	private:
		void InitParseFromHLSL();

		void ShutParseFromHLSL();
	};
}
