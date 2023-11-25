#include <pch.h>
#include <aiva/layer1_ext/shader_meta_for_code.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/shader_consts.h>
#include <aiva/layer1_ext/shader_meta_for_resource.h>
#include <aiva/layer1_ext/shader_meta_for_struct.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderMetaForCode::ShaderMetaForCode(std::string const& text, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		Asserts::CheckBool(!std::empty(text), "Text is not valid");

		{ // Structures
			auto const regexForStructs = std::regex(R"(struct[\S\s]*?\{[\S\s]*?\};)", std::regex::icase | std::regex::optimize);
			for (auto i = std::sregex_iterator{ std::cbegin(text), std::cend(text), regexForStructs }; i != decltype(i){}; i++)
			{
				auto const& matchForStruct = *i;
				Asserts::CheckBool(matchForStruct.ready() && !std::empty(matchForStruct), "Match for struct is not valid");

				auto const stringForStruct = matchForStruct[std::size(matchForStruct) - std::size_t{ 1 }].str();
				Asserts::CheckBool(!std::empty(stringForStruct), "String for struct is not valid");

				auto const& metaForStruct = mStructures.emplace_back(NewObject<ShaderMetaForStruct>(stringForStruct, engine));
				Asserts::CheckBool(metaForStruct, "Meta for struct is not valid");
			}
		}

		{ // Structure For Input
			auto const regexForInput = std::regex(std::string{ ShaderConsts::EntryPointName } + std::string{ R"(\s*?\((\w+)[\S\s]*?\))" }, std::regex::icase | std::regex::optimize);
			auto matchForInput = std::smatch{};

			std::regex_search(text, matchForInput, regexForInput);
			Asserts::CheckBool(matchForInput.ready() && !std::empty(matchForInput), "Match for input is not valid");

			auto const stringForInput = matchForInput[std::size(matchForInput) - std::size_t{ 1 }].str();
			Asserts::CheckBool(!std::empty(stringForInput), "String for input is not valid");

			auto const iterForInput = std::find_if(std::cbegin(mStructures), std::cend(mStructures), [&stringForInput](auto const& metaForStruct)
			{
				Asserts::CheckBool(metaForStruct, "Meta for struct is not valid");
				return metaForStruct->Name() == stringForInput;
			});
			Asserts::CheckBool(iterForInput != std::cend(mStructures), "Iter for input is not valid");

			auto const& valueForInput = (*iterForInput);
			Asserts::CheckBool(valueForInput, "Value for input is not valid");

			valueForInput->SortFields();
			mStructureForInput = valueForInput;
		}

		{ // Structure For Output
			auto const regexForOutput = std::regex(std::string{ R"((\w+?)\s+?)" } + std::string{ ShaderConsts::EntryPointName } + std::string{ R"(\s*?\([\S\s]*?\))" }, std::regex::icase | std::regex::optimize);
			auto matchForOutput = std::smatch{};

			std::regex_search(text, matchForOutput, regexForOutput);
			Asserts::CheckBool(matchForOutput.ready() && !std::empty(matchForOutput), "Match for output is not valid");

			auto const stringForOutput = matchForOutput[std::size(matchForOutput) - std::size_t{ 1 }].str();
			Asserts::CheckBool(!std::empty(stringForOutput), "String for output is not valid");

			auto const iterForOutput = std::find_if(std::cbegin(mStructures), std::cend(mStructures), [&stringForOutput](auto const& metaForStruct)
			{
				Asserts::CheckBool(metaForStruct, "Meta for struct is not valid");
				return metaForStruct->Name() == stringForOutput;
			});
			Asserts::CheckBool(iterForOutput != std::cend(mStructures), "Iter for output is not valid");

			auto const& valueForOutput = (*iterForOutput);
			Asserts::CheckBool(valueForOutput, "Value for output is not valid");

			valueForOutput->SortFields();
			mStructureForOutput = valueForOutput;
		}

		{ // Resources
			auto const regexForResources = std::regex(R"(\S+? +?\w+? *?: *?register *?\([\S\s]*?\);)", std::regex::icase | std::regex::optimize);
			for (auto i = std::sregex_iterator{ std::cbegin(text), std::cend(text), regexForResources }; i != decltype(i){}; i++)
			{
				auto const& matchForResource = (*i);
				Asserts::CheckBool(matchForResource.ready() && !std::empty(matchForResource), "Match for resource is not valid");

				auto const stringForResource = matchForResource[std::size(matchForResource) - std::size_t{ 1 }].str();
				Asserts::CheckBool(!std::empty(stringForResource), "String for resource is not valid");

				auto const& metaForResource = mResources.emplace_back(NewObject<ShaderMetaForResource>(stringForResource, engine));
				Asserts::CheckBool(metaForResource, "Meta for resource is not valid");
			}
		}
	}

	ShaderMetaForCode::~ShaderMetaForCode()
	{

	}
}
