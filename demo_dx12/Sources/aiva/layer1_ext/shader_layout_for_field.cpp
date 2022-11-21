#include <pch.h>
#include <aiva/layer1_ext/shader_layout_for_field.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/shader_layout_for_type.h>
#include <aiva/layer1_ext/shader_meta_for_field.h>
#include <aiva/layer1_ext/shader_meta_for_semantic.h>
#include <aiva/layer1_ext/shader_meta_for_type.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderLayoutForField::ShaderLayoutForField(std::shared_ptr<ShaderMetaForField> const& metaForField, std::size_t const offset, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		Asserts::CheckBool(metaForField, "Meta for field is not valid");

		auto const isPartOfLayout = !metaForField->Semantic() || !metaForField->Semantic()->IsSystemValue();

		{ // Offset
			mOffset = isPartOfLayout ? offset : std::numeric_limits<decltype(mOffset)>::max();
		}

		{ // Type
			auto const& metaForType = metaForField->Type();
			Asserts::CheckBool(metaForType, "Meta for type is not valid");

			auto const layoutForType = NewObject<ShaderLayoutForType>(metaForType, engine);
			Asserts::CheckBool(layoutForType, "Layout for type is not valid");

			mType = layoutForType;
		}

		{ // IsPartOfLayout
			mIsPartOfLayout = isPartOfLayout;
		}
	}

	ShaderLayoutForField::~ShaderLayoutForField()
	{

	}
}
