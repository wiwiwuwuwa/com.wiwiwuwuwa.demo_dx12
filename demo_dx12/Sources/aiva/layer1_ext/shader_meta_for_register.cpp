#include <pch.h>
#include <aiva/layer1_ext/shader_meta_for_register.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/e_shader_register_type_utils.h>
#include <aiva/layer1_ext/shader_struct_register_info.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderMetaForRegister::ShaderMetaForRegister(std::string const& text, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		Asserts::CheckBool(!std::empty(text), "Text is not valid");

		{ // Value
			auto const regexForValue = std::regex(R"(^(\D))", std::regex::icase | std::regex::optimize);
			auto matchForValue = std::smatch{};

			std::regex_search(text, matchForValue, regexForValue);
			Asserts::CheckBool(matchForValue.ready() && !std::empty(matchForValue), "Match for value is not valid");

			auto const stringForValue = matchForValue[std::size(matchForValue) - std::size_t{ 1 }].str();
			Asserts::CheckBool(!std::empty(stringForValue), "String for value is not valid");

			auto const valueForValue = EShaderRegisterTypeUtils::ParseFromHLSL(stringForValue);
			Asserts::CheckBool(EShaderRegisterTypeUtils::IsValid(valueForValue), "Value for value is not valid");

			mValue = valueForValue;
		}

		{ // Index
			auto const regexForIndex = std::regex(R"(^\D(\d+))", std::regex::icase | std::regex::optimize);
			auto matchForIndex = std::smatch{};

			std::regex_search(text, matchForIndex, regexForIndex);
			Asserts::CheckBool(matchForIndex.ready() && !std::empty(matchForIndex), "Match for index is not valid");

			auto const stringForIndex = matchForIndex[std::size(matchForIndex) - std::size_t{ 1 }].str();
			Asserts::CheckBool(!std::empty(stringForIndex), "String for index is not valid");

			auto const valueForIndex = std::stoi(stringForIndex);
			Asserts::CheckBool(valueForIndex >= decltype(valueForIndex) {}, "Value for index is not valid");

			mIndex = static_cast<std::size_t>(valueForIndex);
		}

		{ // Space
			auto const regexForSpace = std::regex(R"(space(\d+))", std::regex::icase | std::regex::optimize);
			auto matchForSpace = std::smatch{};

			std::regex_search(text, matchForSpace, regexForSpace);
			if (matchForSpace.ready() && !std::empty(matchForSpace))
			{
				auto const stringForSpace = matchForSpace[std::size(matchForSpace) - std::size_t{ 1 }].str();
				Asserts::CheckBool(!std::empty(stringForSpace), "String for space is not valid");

				auto const valueForSpace = std::stoi(stringForSpace);
				Asserts::CheckBool(valueForSpace >= decltype(valueForSpace) {}, "Value for space is not valid");

				mSpace = static_cast<std::size_t>(valueForSpace);
			}
			else
			{
				mSpace = {};
			}
		}
	}

	ShaderMetaForRegister::ShaderMetaForRegister(ShaderStructRegisterInfo const& registerInfo, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		mValue = registerInfo.Value();
		mIndex = registerInfo.Index();
		mSpace = registerInfo.Space();
	}

	ShaderMetaForRegister::~ShaderMetaForRegister()
	{

	}

	std::vector<std::shared_ptr<ShaderMetaForRegister>> ShaderMetaForRegister::Convert(std::vector<ShaderStructRegisterInfo> const& registersInfo, aiva::layer1::Engine& engine)
	{
		auto registersMeta = std::vector<std::shared_ptr<ShaderMetaForRegister>>{};

		for (auto const& registerInfo : registersInfo)
		{
			registersMeta.emplace_back(NewObject<ShaderMetaForRegister>(registerInfo, engine));
		}

		return registersMeta;
	}
}
