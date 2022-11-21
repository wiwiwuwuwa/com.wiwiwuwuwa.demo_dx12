#include <pch.h>
#include <aiva/layer1_ext/shader_meta_for_struct.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/shader_meta_for_field.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderMetaForStruct::ShaderMetaForStruct(std::string const& text, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		Asserts::CheckBool(!std::empty(text), "Text is not valid");

		{ // Name
			auto const regexForName = std::regex(R"((?:struct\s+)(\S+)(?:\s+))", std::regex::icase | std::regex::optimize);
			auto matchForName = std::smatch{};

			std::regex_search(text, matchForName, regexForName);
			Asserts::CheckBool(matchForName.ready() && !std::empty(matchForName), "Match for name is not valid");

			auto const stringForName = matchForName[std::size(matchForName) - 1].str();
			Asserts::CheckBool(!std::empty(stringForName), "String for name is not valid");

			mName = stringForName;
		}

		{ // Fields
			auto const regexForFields = std::regex(R"(\w[\w :]+?;)", std::regex::icase | std::regex::optimize);
			for (auto i = std::sregex_iterator{ std::cbegin(text), std::cend(text), regexForFields }; i != decltype(i){}; i++)
			{
				auto const& matchForField = *i;
				Asserts::CheckBool(matchForField.ready() && !std::empty(matchForField), "Match for field is not valid");

				auto const stringForField = matchForField[std::size(matchForField) - 1].str();
				Asserts::CheckBool(!std::empty(stringForField), "String for field is not valid");

				auto const& metaForField = mFields.emplace_back(NewObject<ShaderMetaForField>(stringForField, engine));
				Asserts::CheckBool(metaForField, "Meta for field is not valid");
			}
		}
	}

	ShaderMetaForStruct::~ShaderMetaForStruct()
	{

	}

	void ShaderMetaForStruct::SortFields()
	{
		std::sort(std::begin(mFields), std::end(mFields), [](auto const& fieldA, auto const& fieldB)
		{
			Asserts::CheckBool(fieldA, "Field A is not valid");
			Asserts::CheckBool(fieldB, "Field B is not valid");
			return (*fieldA) < (*fieldB);
		});
	}
}
