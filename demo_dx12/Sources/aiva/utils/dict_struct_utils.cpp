#include <pch.h>
#include <aiva/utils/dict_struct_utils.h>

#include <aiva/utils/layout_field.h>
#include <aiva/utils/layout_struct.h>
#include <aiva/utils/layout_struct_utils.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/boxed_value_utils.h>
#include <aiva/utils/dict_struct.h>
#include <aiva/utils/i_boxed_value.h>
#include <aiva/utils/meta_field.h>
#include <aiva/utils/meta_struct.h>
#include <aiva/utils/meta_struct_utils.h>
#include <aiva/utils/object_utils.h>

bool aiva::utils::DictStructUtils::HasSameFields(DictStructPointerType const& structA, DictStructPointerType const structB)
{
	if (structA == structB)
	{
		return true;
	}

	if (!(structA && structB))
	{
		return false;
	}

	if (std::size(structA->FieldsBoxed()) != std::size(structB->FieldsBoxed()))
	{
		return false;
	}

	for (auto const& pair : structA->FieldsBoxed())
	{
		auto const& fieldName = pair.first;

		auto const& fieldAValue = pair.second;
		if (!fieldAValue)
		{
			return false;
		}

		auto const fieldBValue = structB->FieldBoxed(fieldName);
		if (!fieldBValue)
		{
			return false;
		}

		if (fieldAValue->Type() != fieldBValue->Type())
		{
			return false;
		}
	}

	return true;
}

bool aiva::utils::DictStructUtils::IsMatchingLayout(DictStructPointerType const& dstStruct, MetaStructPointerType const& srcLayout)
{
	if (!dstStruct || !srcLayout)
	{
		return false;
	}

	for (std::size_t i = {}; i < srcLayout->Num(); i++)
	{
		auto const& srcField = srcLayout->Get(i);
		Asserts::CheckBool(srcField, "Src field is not valid");

		auto const& dstField = dstStruct->FieldBoxed(srcField->Name());
		if (!dstField)
		{
			return false;
		}

		if (!BoxedValueUtils::IsMatchingLayout(dstField, srcField))
		{
			return false;
		}
	}

	return true;
}

aiva::utils::DictStructUtils::DictStructPointerType aiva::utils::DictStructUtils::GenerateFrom(AlignStructPointerType const& layoutStruct)
{
	Asserts::CheckBool(layoutStruct, "Layout struct is not valid");

	auto const dictStruct = NewObject<DictStructElementType>();
	Asserts::CheckBool(dictStruct, "Dict struct is not valid");

	for (auto const& pair : layoutStruct->Fields())
	{
		auto const& layoutName = pair.first;
		auto const& layoutField = pair.second;
		Asserts::CheckBool(layoutField, "Layout field is not valid");

		auto const dictField = BoxedValueUtils::CreateInstance(layoutField->Type());
		Asserts::CheckBool(dictField, "Dict field is not valid");

		dictStruct->FieldBoxed(layoutName, dictField);
	}

	return dictStruct;
}

aiva::utils::DictStructUtils::DictStructPointerType aiva::utils::DictStructUtils::GenerateFrom(MetaStructPointerType const& metaStruct)
{
	Asserts::CheckBool(metaStruct, "Meta struct is not valid");

	auto const dictStruct = NewObject<DictStructElementType>();
	Asserts::CheckBool(dictStruct, "Dict struct is not valid");

	for (std::size_t i = {}; i < metaStruct->Num(); i++)
	{
		auto const& metaField = metaStruct->Get(i);
		Asserts::CheckBool(metaField, "Meta field is not valid");

		auto const dictField = BoxedValueUtils::CreateInstance(metaField->Type());
		Asserts::CheckBool(dictField, "Dict field is not valid");

		dictStruct->FieldBoxed(metaField->Name(), dictField);
	}

	return dictStruct;
}

std::vector<std::byte> aiva::utils::DictStructUtils::SerializeToBinary(DictStructPointerType const& dictStruct)
{
	Asserts::CheckBool(dictStruct, "Dict struct is not valid");

	auto const alignStruct = LayoutStructUtils::GenerateFrom(dictStruct);
	Asserts::CheckBool(alignStruct, "Align struct is not valid");

	auto const structBinary = SerializeToBinary(dictStruct, alignStruct);
	Asserts::CheckBool(!std::empty(structBinary), "Struct binary is empty");

	return structBinary;
}

std::vector<std::byte> aiva::utils::DictStructUtils::SerializeToBinary(DictStructPointerType const& dictStruct, MetaStructPointerType const& metaStruct)
{
	Asserts::CheckBool(dictStruct, "Dict struct is not valid");
	Asserts::CheckBool(metaStruct, "Meta struct is not valid");

	auto const layoutStruct = LayoutStructUtils::GenerateFrom(metaStruct);
	Asserts::CheckBool(layoutStruct, "Layout struct is not valid");

	auto const structBinary = SerializeToBinary(dictStruct, layoutStruct);
	Asserts::CheckBool(!std::empty(structBinary), "Struct binary is empty");

	return structBinary;
}

std::vector<std::byte> aiva::utils::DictStructUtils::SerializeToBinary(DictStructPointerType const& dictStruct, AlignStructPointerType const& alignStruct)
{
	Asserts::CheckBool(dictStruct, "Dict struct is not valid");
	Asserts::CheckBool(alignStruct, "Align struct is not valid");

	Asserts::CheckBool(alignStruct->Offset() >= 0, "Align struct offset is not valid");
	Asserts::CheckBool(alignStruct->Size() > 0, "Align struct size is not valid");
	auto structBinary = std::vector<std::byte>{ alignStruct->Size() };

	for (auto const& pair : alignStruct->Fields())
	{
		auto const& fieldName = pair.first;
		auto const& fieldInfo = pair.second;

		Asserts::CheckBool(fieldInfo, "Field info is not valid");
		Asserts::CheckBool(fieldInfo->Offset() >= 0, "Field info is more to the left than the struct info");
		Asserts::CheckBool(fieldInfo->Size() > 0, "Field info size is not valid");
		Asserts::CheckBool(fieldInfo->Offset() + fieldInfo->Size() <= alignStruct->Size(), "Field info is more to the right than the struct info");

		auto const dictField = dictStruct->FieldBoxed(fieldName);
		Asserts::CheckBool(dictField, "Dict field is not valid");
		Asserts::CheckBool(fieldInfo->Type() == EBoxedType::UNKNOWN || fieldInfo->Type() == dictField->Type(), "Dict field has the wrong type");

		auto const fieldBinary = dictField->Binary();
		Asserts::CheckBool(!std::empty(fieldBinary), "Field binary is empty");
		Asserts::CheckBool(fieldInfo->Size() == std::size(fieldBinary), "Field binary has the different size than the field info");

		auto const srcSpan = fieldBinary;
		auto const dstSpan = boost::span{ structBinary }.subspan(fieldInfo->Offset(), fieldInfo->Size());

		Asserts::CheckBool(memcpy_s(std::data(dstSpan), std::size(dstSpan), std::data(srcSpan), std::size(srcSpan)) == 0, "Failed to memcpy_s");
	}

	return structBinary;
}

aiva::utils::DictStructUtils::DictStructPointerType aiva::utils::DictStructUtils::DeserealizeFromBinary(MetaStructPointerType const& metaStruct, boost::span<const std::byte> const& structBinary)
{
	Asserts::CheckBool(metaStruct, "Meta struct is not valid");
	Asserts::CheckBool(!std::empty(structBinary), "Struct binary is not valid");

	auto const layoutStruct = LayoutStructUtils::GenerateFrom(metaStruct);
	Asserts::CheckBool(layoutStruct, "Layout struct is not valid");

	auto const dictStruct = DeserealizeFromBinary(layoutStruct, structBinary);
	Asserts::CheckBool(dictStruct, "Dict struct is not valid");

	return dictStruct;
}

aiva::utils::DictStructUtils::DictStructPointerType aiva::utils::DictStructUtils::DeserealizeFromBinary(AlignStructPointerType const& alignStruct, boost::span<const std::byte> const& structBinary)
{
	Asserts::CheckBool(alignStruct, "Align struct is not valid");
	Asserts::CheckBool(alignStruct->Offset() >= 0, "Align struct offset is not valid");
	Asserts::CheckBool(alignStruct->Size() > 0, "Align struct size is not valid");

	Asserts::CheckBool(!std::empty(structBinary), "Struct binary is empty");
	Asserts::CheckBool(alignStruct->Size() == std::size(structBinary), "Struct binary has the different size than the struct info");

	auto const dictStruct = NewObject<DictStructElementType>();
	Asserts::CheckBool(dictStruct, "Dict struct is not valid");

	for (auto const& pair : alignStruct->Fields())
	{
		auto const& fieldName = pair.first;
		auto const& fieldInfo = pair.second;

		Asserts::CheckBool(fieldInfo, "Field info is not valid");
		Asserts::CheckBool(fieldInfo->Type() != EBoxedType::UNKNOWN, "Field info type is not valid");
		Asserts::CheckBool(fieldInfo->Offset() >= 0, "Field info is more to the left than the struct info");
		Asserts::CheckBool(fieldInfo->Size() > 0, "Field info size is not valid");
		Asserts::CheckBool(fieldInfo->Offset() + fieldInfo->Size() <= alignStruct->Size(), "Field info is more to the right than the struct info");

		auto const dictField = BoxedValueUtils::CreateInstance(fieldInfo->Type());
		Asserts::CheckBool(dictField, "Dict field is not valid");
		Asserts::CheckBool(fieldInfo->Type() == dictField->Type(), "Dict field has the wrong type");

		auto const fieldBinary = dictField->Binary();
		Asserts::CheckBool(!std::empty(fieldBinary), "Field binary is empty");
		Asserts::CheckBool(fieldInfo->Size() == std::size(fieldBinary), "Field binary has the different size than the field info");

		auto const srcSpan = boost::span{ structBinary }.subspan(fieldInfo->Offset(), fieldInfo->Size());
		auto const dstSpan = fieldBinary;

		Asserts::CheckBool(memcpy_s(std::data(dstSpan), std::size(dstSpan), std::data(srcSpan), std::size(srcSpan)) == 0, "Failed to memcpy_s");
		dictStruct->FieldBoxed(fieldName, dictField);
	}

	return dictStruct;
}
