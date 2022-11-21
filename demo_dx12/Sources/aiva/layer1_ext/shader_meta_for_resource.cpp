#include <pch.h>
#include <aiva/layer1_ext/shader_meta_for_resource.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/shader_meta_for_register.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderMetaForResource::ShaderMetaForResource(std::string const& text, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		Asserts::CheckBool(!std::empty(text), "Text is not valid");

		{ // Name
			auto const regexForName = std::regex(R"((\w+?) *?:)", std::regex::icase | std::regex::optimize);
			auto matchForName = std::smatch{};

			std::regex_search(text, matchForName, regexForName);
			Asserts::CheckBool(matchForName.ready() && !std::empty(matchForName), "Match for name is not valid");

			auto const stringForName = matchForName[std::size(matchForName) - std::size_t{ 1 }].str();
			Asserts::CheckBool(!std::empty(stringForName), "String for name is not valid");

			mName = stringForName;
		}

		{ // Register
			auto const regexForRegister = std::regex(R"(register\(([\S\s]*?)\))", std::regex::icase | std::regex::optimize);
			auto matchForRegister = std::smatch{};

			std::regex_search(text, matchForRegister, regexForRegister);
			Asserts::CheckBool(matchForRegister.ready() && !std::empty(matchForRegister), "Match for register is not valid");

			auto const stringForRegister = matchForRegister[std::size(matchForRegister) - std::size_t{ 1 }].str();
			Asserts::CheckBool(!std::empty(stringForRegister), "String for register is not valid");

			auto const valueForRegister = NewObject<ShaderMetaForRegister>(stringForRegister, engine);
			Asserts::CheckBool(valueForRegister, "Value for register is not valid");

			mRegister = valueForRegister;
		}
	}

	ShaderMetaForResource::~ShaderMetaForResource()
	{

	}
}
