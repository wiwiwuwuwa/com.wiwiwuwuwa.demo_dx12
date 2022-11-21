#include <pch.h>
#include <aiva/layer1_ext/shader_consts.h>

#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	ShaderConsts::ShaderConsts()
	{
		InitCompareHLSLSemantics();
		InitParseFromGLTF();
	}

	ShaderConsts::~ShaderConsts()
	{
		ShutParseFromGLTF();
		ShutCompareHLSLSemantics();
	}

	ShaderConsts& ShaderConsts::Instance()
	{
		static ShaderConsts instance{};
		return instance;
	}

	bool ShaderConsts::IsSemanticUsedForInputAssembly(std::string const& semantic)
	{
		auto const iterator = std::find(std::cbegin(SemanticsForInputAssembly), std::cend(SemanticsForInputAssembly), semantic);
		auto const hasValue = iterator != std::cend(SemanticsForInputAssembly);

		return hasValue;
	}

	std::string const& ShaderConsts::ParseFromGLTF(std::string const& gltf)
	{
		auto const iter = Instance().mGLTFToHLSL.find(gltf);
		Asserts::CheckBool(iter != std::cend(Instance().mGLTFToHLSL), "Failed to parse HLSL from GLTF");

		auto const& hlsl = iter->second;
		Asserts::CheckBool(!std::empty(hlsl), "HLSL is not valid");

		return hlsl;
	}

	void ShaderConsts::InitParseFromGLTF()
	{
		mGLTFToHLSL = {};

		mGLTFToHLSL.insert_or_assign("POSITION", "POSITION");
		mGLTFToHLSL.insert_or_assign("NORMAL", "NORMAL");
		mGLTFToHLSL.insert_or_assign("TEXCOORD", "TEXCOORD");
		mGLTFToHLSL.insert_or_assign("COLOR", "COLOR");
	}

	void ShaderConsts::ShutParseFromGLTF()
	{
		mGLTFToHLSL = {};
	}

	bool ShaderConsts::CompareHLSLSemantics(std::string const& hlslA, std::string const& hlslB)
	{
		auto const iterA = Instance().mSemanticToSortingOrder.find(hlslA);
		auto const iterB = Instance().mSemanticToSortingOrder.find(hlslB);

		Asserts::CheckBool(iterA != std::cend(Instance().mSemanticToSortingOrder), "Iter A is not valid");
		Asserts::CheckBool(iterB != std::cend(Instance().mSemanticToSortingOrder), "Iter B is not valid");

		auto const valueA = iterA->second;
		auto const valueB = iterB->second;

		return valueA < valueB;
	}

	bool ShaderConsts::CompareGLTFSemantics(std::string const& gltfA, std::string const& gltfB)
	{
		Asserts::CheckBool(!std::empty(gltfA), "GLTF A is not valid");
		Asserts::CheckBool(!std::empty(gltfB), "GLTF B is not valid");

		auto const& hlslA = ParseFromGLTF(gltfA);
		auto const& hlslB = ParseFromGLTF(gltfB);

		Asserts::CheckBool(!std::empty(hlslA), "HLSL A is not valid");
		Asserts::CheckBool(!std::empty(hlslB), "HLSL B is not valid");

		return CompareHLSLSemantics(hlslA, hlslB);
	}

	void ShaderConsts::InitCompareHLSLSemantics()
	{
		mSemanticToSortingOrder = {};

		for (auto const& semantic : SemanticsForInputAssembly)
		{
			mSemanticToSortingOrder.insert_or_assign(semantic, std::size(mSemanticToSortingOrder));
		}
	}

	void ShaderConsts::ShutCompareHLSLSemantics()
	{
		mSemanticToSortingOrder = {};
	}
}
