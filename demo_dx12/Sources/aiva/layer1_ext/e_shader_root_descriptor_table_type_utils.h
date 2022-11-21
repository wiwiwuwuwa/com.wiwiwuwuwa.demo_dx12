#pragma once
#include <pch.h>

#include <aiva/layer1_ext/e_shader_register_type.h>
#include <aiva/layer1_ext/e_shader_root_descriptor_table_type.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct EShaderRootDescriptorTableTypeUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		EShaderRootDescriptorTableTypeUtils();

		~EShaderRootDescriptorTableTypeUtils();

	private:
		static EShaderRootDescriptorTableTypeUtils& Instance();

	// ----------------------------------------------------
	// Validation

	public:
		static bool IsValid(EShaderRootDescriptorTableType const value);

	// ----------------------------------------------------
	// Iteration

	public:
		static constexpr EShaderRootDescriptorTableType Begin();

		static constexpr EShaderRootDescriptorTableType End();

	// ----------------------------------------------------
	// EShaderRegisterType

	public:
		M_OBJECT_FIELD_VAR_3(private, M_SINGLE_ARG(std::unordered_map<EShaderRegisterType, EShaderRootDescriptorTableType>), ShaderRegisterTypeToShaderRootDescriptorTableType);

	public:
		static EShaderRootDescriptorTableType Parse(EShaderRegisterType const shaderRegisterType);

	private:
		void InitParseFromShaderRegisterType();

		void ShutParseFromShaderRegisterType();
	};
}

// --------------------------------------------------------

#include <aiva/utils_ext/enum_utils.h>

namespace aiva::layer1_ext
{
	constexpr EShaderRootDescriptorTableType EShaderRootDescriptorTableTypeUtils::Begin()
	{
		return aiva::utils_ext::EnumUtils::AddByVal(EShaderRootDescriptorTableType::Unknown, 1);
	}

	constexpr EShaderRootDescriptorTableType EShaderRootDescriptorTableTypeUtils::End()
	{
		return EShaderRootDescriptorTableType::Max;
	}
}
