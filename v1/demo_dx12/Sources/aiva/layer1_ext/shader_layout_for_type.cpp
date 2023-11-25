#include <pch.h>
#include <aiva/layer1_ext/shader_layout_for_type.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/shader_meta_for_type.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/e_value_type_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderLayoutForType::ShaderLayoutForType(std::shared_ptr<ShaderMetaForType> const& metaForType, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		Asserts::CheckBool(metaForType, "Meta for type is not valid");

		{ // Size
			auto const type = metaForType->Name();
			Asserts::CheckBool(EValueTypeUtils::IsValid(type), "Type is not valid");

			auto const size = EValueTypeUtils::SizeOf(type);
			Asserts::CheckBool(size > decltype(size){}, "Size is not valid");

			mSize = size;
		}
	}

	ShaderLayoutForType::~ShaderLayoutForType()
	{

	}
}
