#pragma once
#include <pch.h>

#include <aiva/layer1_ext/e_shader_register_type.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct DxDescriptorRangeTypeUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		DxDescriptorRangeTypeUtils();

		~DxDescriptorRangeTypeUtils();

	private:
		static DxDescriptorRangeTypeUtils& Instance();

	// ----------------------------------------------------
	// EShaderRegisterType Parsing

	public:
		M_OBJECT_FIELD_VAR_3(private, M_SINGLE_ARG(std::unordered_map<EShaderRegisterType, D3D12_DESCRIPTOR_RANGE_TYPE>), ShaderRegisterTypeToDescriptorRangeType);

	public:
		static D3D12_DESCRIPTOR_RANGE_TYPE Parse(EShaderRegisterType const shaderRegisterType);

	private:
		void InitParseFromShaderRegisterType();

		void ShutParseFromShaderRegisterType();
	};
}
