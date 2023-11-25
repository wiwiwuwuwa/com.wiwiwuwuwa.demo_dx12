#include <pch.h>
#include <aiva/layer1_ext/shader_meta_for_type.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/e_value_type_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderMetaForType::ShaderMetaForType(std::string const& text, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		Asserts::CheckBool(!std::empty(text), "Text is not valid");

		{ // Name
			auto const name = EValueTypeUtils::ParseFromHLSL(text);
			Asserts::CheckBool(EValueTypeUtils::IsValid(name), "Name is not valid");

			mName = name;
		}
	}

	ShaderMetaForType::~ShaderMetaForType()
	{

	}
}
