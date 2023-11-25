#include <pch.h>
#include <aiva/layer1_ext/shader_meta_for_semantic.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/shader_consts.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/enum_utils.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderMetaForSemantic::ShaderMetaForSemantic(std::string const& text, ShaderParsingRulesForSemantic const rules, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		Asserts::CheckBool(!std::empty(text), "Text is not valid");

		{ // Value
			auto const regexForValue = std::regex(R"([a-z]+)", std::regex::icase | std::regex::optimize);
			auto matchForValue = std::smatch{};

			std::regex_search(text, matchForValue, regexForValue);
			Asserts::CheckBool(matchForValue.ready() && !std::empty(matchForValue), "Match for value is not valid");

			auto const gltfValue = matchForValue[std::size(matchForValue) - std::size_t{ 1 }].str();
			Asserts::CheckBool(!std::empty(gltfValue), "GLTF value is not valid");

			switch (rules)
			{
			case decltype(rules)::HLSL:
			{
				mValue = gltfValue;
				break;
			}

			case decltype(rules)::GLTF:
			{
				auto const& hlslValue = ShaderConsts::ParseFromGLTF(gltfValue);
				Asserts::CheckBool(!std::empty(hlslValue), "HLSL value is not valid");

				mValue = hlslValue;
				break;
			}

			default:
				Asserts::CheckBool("Unsupported shader parsing rules for semantic");
				break;
			}
		}

		{ // Index
			auto const regexForIndex = std::regex(R"(_(\d+))", std::regex::icase | std::regex::optimize);
			auto matchForIndex = std::smatch{};

			std::regex_search(text, matchForIndex, regexForIndex);
			if (matchForIndex.ready() && !std::empty(matchForIndex))
			{
				auto const stringForIndex = matchForIndex[std::size(matchForIndex) - std::size_t{ 1 }].str();
				Asserts::CheckBool(!std::empty(stringForIndex), "String for index is not valid");

				auto const valueForIndex = std::stoi(stringForIndex);
				Asserts::CheckBool(valueForIndex >= decltype(valueForIndex) {}, "Value for index is not valid");

				mIndex = static_cast<std::size_t>(valueForIndex);
			}
			else
			{
				mIndex = {};
			}
		}

		{ // Is Input Assembly
			mIsInputAssembly = ShaderConsts::IsSemanticUsedForInputAssembly(mValue);
		}

		{ // Is System Value
			auto const regexForSystemValue = std::regex(R"(^sv_)", std::regex::icase | std::regex::optimize);
			mIsSystemValue = std::regex_search(text, regexForSystemValue);
		}
	}

	ShaderMetaForSemantic::~ShaderMetaForSemantic()
	{

	}

	bool ShaderMetaForSemantic::operator<(ShaderMetaForSemantic const& other) const
	{
		if (mIsInputAssembly != other.mIsInputAssembly)
		{
			return mIsInputAssembly > other.mIsInputAssembly;
		}

		if (mIsSystemValue != other.mIsSystemValue)
		{
			return mIsSystemValue < other.mIsSystemValue;
		}

		if (mValue != other.mValue)
		{
			return ShaderConsts::CompareHLSLSemantics(mValue, other.mValue);
		}

		if (mIndex != other.mIndex)
		{
			return mIndex < other.mIndex;
		}

		return false;
	}
}
