#pragma once
#include <pch.h>

#include <aiva/layer1_ext/gl_type_desc.h>
#include <aiva/utils_ext/e_value_type.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::utils_ext
{
	struct EValueTypeUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		EValueTypeUtils();

		~EValueTypeUtils();

	private:
		static EValueTypeUtils& Instance();

	// ----------------------------------------------------
	// Validation

	public:
		static bool IsValid(EValueType const value);

	// ----------------------------------------------------
	// Size Of

	public:
		M_OBJECT_FIELD_VAR_3(private, M_SINGLE_ARG(std::unordered_map<EValueType, std::size_t>), ValueTypeToSize);

	public:
		static std::size_t SizeOf(EValueType const valueType);

		static std::size_t SizeOf(aiva::layer1_ext::GlTypeDesc const& gltf);

	private:
		void InitSizeOf();

		void ShutSizeOf();

	// ----------------------------------------------------
	// HLSL Parsing

	public:
		M_OBJECT_FIELD_VAR_3(private, M_SINGLE_ARG(std::unordered_map<std::string, EValueType>), HLSLToValueType);

	public:
		static EValueType ParseFromHLSL(std::string const& typeName);

	private:
		void InitParseFromHLSL();

		void ShutParseFromHLSL();

	// ----------------------------------------------------
	// GLTF Parsing

	public:
		M_OBJECT_FIELD_VAR_3(private, M_SINGLE_ARG(std::unordered_map<aiva::layer1_ext::GlTypeDesc, EValueType, boost::hash<aiva::layer1_ext::GlTypeDesc>>), GLTFToValueType);

	public:
		static EValueType ParseFromGLTF(aiva::layer1_ext::GlTypeDesc const& gltf);

	private:
		void InitParseFromGLTF();

		void ShutParseFromGLTF();
	};
}
