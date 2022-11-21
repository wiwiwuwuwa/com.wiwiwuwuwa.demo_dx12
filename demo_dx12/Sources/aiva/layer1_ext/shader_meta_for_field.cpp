#include <pch.h>
#include <aiva/layer1_ext/shader_meta_for_field.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/shader_meta_for_semantic.h>
#include <aiva/layer1_ext/shader_meta_for_struct.h>
#include <aiva/layer1_ext/shader_meta_for_type.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderMetaForField::ShaderMetaForField(std::string const& text, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		Asserts::CheckBool(!std::empty(text), "Text is not valid");

		{ // Type
			auto const regexForType = std::regex(R"(^\w+)", std::regex::icase | std::regex::optimize);
			auto matchForType = std::smatch{};

			std::regex_search(text, matchForType, regexForType);
			Asserts::CheckBool(matchForType.ready() && !std::empty(matchForType), "Match for type is not valid");

			auto const stringForType = matchForType[std::size(matchForType) - std::size_t{ 1 }].str();
			Asserts::CheckBool(!std::empty(stringForType), "String for type is not valid");

			auto const valueForType = NewObject<ShaderMetaForType>(stringForType, engine);
			Asserts::CheckBool(valueForType, "Value for type is not valid");

			mType = valueForType;
		}

		{ // Name
			auto const regexForName = std::regex(R"(^\w+? +?(\w+))", std::regex::icase | std::regex::optimize);
			auto matchForName = std::smatch{};

			std::regex_search(text, matchForName, regexForName);
			Asserts::CheckBool(matchForName.ready() && !std::empty(matchForName), "Match for name is not valid");

			auto const stringForName = matchForName[std::size(matchForName) - std::size_t{ 1 }].str();
			Asserts::CheckBool(!std::empty(stringForName), "String for name is not valid");

			mName = stringForName;
		}

		{ // Semantic
			auto const regexForSemantic = std::regex(R"(:\s*?(\w+)\s*?;)", std::regex::icase | std::regex::optimize);
			auto matchForSemantic = std::smatch{};

			std::regex_search(text, matchForSemantic, regexForSemantic);
			if (matchForSemantic.ready() && !std::empty(matchForSemantic))
			{
				auto const stringForSemantic = matchForSemantic[std::size(matchForSemantic) - std::size_t{ 1 }].str();
				Asserts::CheckBool(!std::empty(stringForSemantic), "String for semantic is not valid");

				auto const valueForSemantic = NewObject<ShaderMetaForSemantic>(stringForSemantic, ShaderParsingRulesForSemantic::HLSL, engine);
				Asserts::CheckBool(valueForSemantic, "Value for semantic is not valid");

				mSemantic = valueForSemantic;
			}
			else
			{
				mSemantic = {};
			}
		}
	}

	ShaderMetaForField::~ShaderMetaForField()
	{

	}

	bool ShaderMetaForField::operator<(ShaderMetaForField const& other) const
	{
		if (mSemantic && other.mSemantic)
		{
			return (*mSemantic) < (*other.mSemantic);
		}
		else
		{
			return mName < other.mName;
		}
	}
}
