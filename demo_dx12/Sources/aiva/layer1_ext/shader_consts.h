#pragma once
#include <pch.h>

#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct ShaderConsts final
	{
	// ----------------------------------------------------
	// Main

	private:
		ShaderConsts();

		~ShaderConsts();

	private:
		static ShaderConsts& Instance();

	// ----------------------------------------------------
	// Generic Fields

	public:
		static constexpr std::string::const_pointer const EntryPointName = "main";

		static constexpr std::string::const_pointer const SemanticsForInputAssembly[] =
		{
			"POSITION",
			"NORMAL",
			"TEXCOORD",
			"COLOR",
		};

	public:
		static bool IsSemanticUsedForInputAssembly(std::string const& semantic);

	// ----------------------------------------------------
	// GLTF Parsing

	public:
		M_OBJECT_FIELD_VAR_3(private, M_SINGLE_ARG(std::unordered_map<std::string, std::string>), GLTFToHLSL);

	public:
		static std::string const& ParseFromGLTF(std::string const& gltf);

	private:
		void InitParseFromGLTF();

		void ShutParseFromGLTF();

	// ----------------------------------------------------
	// Sorting Fields

	public:
		M_OBJECT_FIELD_VAR_3(private, M_SINGLE_ARG(std::unordered_map<std::string, std::size_t>), SemanticToSortingOrder);

	public:
		static bool CompareHLSLSemantics(std::string const& hlslA, std::string const& hlslB);

		static bool CompareGLTFSemantics(std::string const& gltfA, std::string const& gltfB);

	private:
		void InitCompareHLSLSemantics();

		void ShutCompareHLSLSemantics();
	};
}
