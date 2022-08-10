#include <pch.h>
#include <aiva/utils/layout_struct_utils.h>

#include <aiva/layer1/shader_consts.h>
#include <aiva/utils/layout_field.h>
#include <aiva/utils/layout_field_utils.h>
#include <aiva/utils/layout_struct.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/boxed_value_utils.h>
#include <aiva/utils/dict_struct.h>
#include <aiva/utils/math_utils.h>
#include <aiva/utils/meta_field.h>
#include <aiva/utils/meta_struct.h>
#include <aiva/utils/meta_struct_utils.h>
#include <aiva/utils/object_utils.h>

bool aiva::utils::LayoutStructUtils::IsMatchingLayout(AlignStructPointerType const& dstStruct, MetaStructPointerType const& srcLayout)
{
	if (!dstStruct || !srcLayout)
	{
		return false;
	}

	for (std::size_t i = {}; i < srcLayout->Num(); i++)
	{
		auto const& srcField = srcLayout->Get(i);
		Asserts::CheckBool(srcField, "Src field is not valid");

		auto const dstField = dstStruct->Field(srcField->Name());
		if (!dstField)
		{
			return false;
		}

		if (!LayoutFieldUtils::IsMatchingLayout(dstField, srcField))
		{
			return false;
		}
	}

	return true;
}

aiva::utils::LayoutStructUtils::AlignStructPointerType aiva::utils::LayoutStructUtils::GenerateFrom(DictStructPointerType const& dictStruct)
{
	Asserts::CheckBool(dictStruct, "Dict struct is not valid");

	auto const metaStruct = MetaStructUtils::GenerateFrom(dictStruct);
	Asserts::CheckBool(metaStruct, "Meta struct is not valid");

	auto const alignStruct = GenerateFrom(metaStruct);
	Asserts::CheckBool(alignStruct, "Align struct is not valid");

	return alignStruct;
}

aiva::utils::LayoutStructUtils::AlignStructPointerType aiva::utils::LayoutStructUtils::GenerateFrom(MetaStructPointerType const& metaStruct)
{
	Asserts::CheckBool(metaStruct, "Meta struct is not valid");

	auto const alignStruct = NewObject<AlignStructElementType>();
	Asserts::CheckBool(alignStruct, "Align struct is not valid");

	for (std::size_t i = {}; i < metaStruct->Num(); i++)
	{
		auto const& metaField = metaStruct->Get(i);
		Asserts::CheckBool(metaField, "Meta field is not valid");

		auto const fieldType = metaField->Type();
		auto const fieldSize = BoxedValueUtils::SizeOf(fieldType);
		auto const fieldOffset = MathUtils::Align(alignStruct->Size(), fieldSize, aiva::layer1::ShaderConsts::VECTOR_SIZE);
		alignStruct->Size(fieldOffset + fieldSize);

		auto const alignField = NewObject<AlignFieldElementType>();
		Asserts::CheckBool(alignField, "Align field is not valid");

		alignField->Type(fieldType);
		alignField->Offset(fieldOffset);
		alignField->Size(fieldSize);

		auto const& fieldName = metaField->Name();
		alignStruct->Field(fieldName, alignField);
	}

	{
		auto const pointer = MathUtils::Align(alignStruct->Size(), aiva::layer1::ShaderConsts::VECTOR_SIZE);
		alignStruct->Size(pointer);
	}

	return alignStruct;
}
