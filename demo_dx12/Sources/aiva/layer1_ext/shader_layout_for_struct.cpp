#include <pch.h>
#include <aiva/layer1_ext/shader_layout_for_struct.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/shader_layout_for_field.h>
#include <aiva/layer1_ext/shader_layout_for_type.h>
#include <aiva/layer1_ext/shader_meta_for_field.h>
#include <aiva/layer1_ext/shader_meta_for_struct.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/enum_utils.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderLayoutForStruct::ShaderLayoutForStruct(std::shared_ptr<ShaderMetaForStruct> const& metaForStruct, std::size_t const offset, ShaderPackingRulesForStruct const rules, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		Asserts::CheckBool(metaForStruct, "Meta for struct is not valid");
		Asserts::CheckBool(EnumUtils::IsValid(rules), "Rules are not valid");

		InitFieldsBeforeRules(offset);

		{ // Layout
			auto const& metaForFields = metaForStruct->Fields();
			Asserts::CheckBool(!std::empty(metaForFields), "Meta for fields is not valid");

			switch (rules)
			{
			case decltype(rules)::Tight:
				InitForTightPackingRules(metaForFields);
				break;

			default:
				Asserts::CheckBool(false, "Packing rules for struct are not valid");
				break;
			}
		}
	}

	ShaderLayoutForStruct::~ShaderLayoutForStruct()
	{

	}

	void ShaderLayoutForStruct::InitFieldsBeforeRules(std::size_t const offset)
	{
		{ // Offset
			mOffset = offset;
		}
	}

	void ShaderLayoutForStruct::InitForTightPackingRules(std::vector<std::shared_ptr<ShaderMetaForField>> const metaForFields)
	{
		Asserts::CheckBool(!std::empty(metaForFields), "Meta for fields is not valid");
		mFields = {};

		auto ptr = std::size_t{};
		for (auto const& metaForField : metaForFields)
		{
			Asserts::CheckBool(metaForField, "Meta for field is not valid");

			auto const layoutForField = NewObject<ShaderLayoutForField>(metaForField, ptr, mEngine);
			Asserts::CheckBool(layoutForField, "Layout for field is not valid");

			mFields.emplace_back(layoutForField);

			if (!layoutForField->IsPartOfLayout())
			{
				continue;
			}

			auto const& layoutForType = layoutForField->Type();
			Asserts::CheckBool(layoutForType, "Layout for type is not valid");

			auto const layoutForSize = layoutForType->Size();
			Asserts::CheckBool(layoutForSize > decltype(layoutForSize){}, "Layout for size is not valid");

			ptr += layoutForSize;
		}

		Asserts::CheckBool(ptr > decltype(ptr){}, "Ptr is not valid");
		mSize = ptr;
	}
}
