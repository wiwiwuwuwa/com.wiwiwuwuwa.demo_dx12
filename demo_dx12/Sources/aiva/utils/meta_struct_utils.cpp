#include <pch.h>
#include <aiva/utils/meta_struct_utils.h>

#include <aiva/utils/asserts.h>
#include <aiva/utils/dict_struct.h>
#include <aiva/utils/layout_field.h>
#include <aiva/utils/layout_struct.h>
#include <aiva/utils/meta_field.h>
#include <aiva/utils/meta_struct.h>
#include <aiva/utils/object_utils.h>

aiva::utils::MetaStructUtils::MetaStructPointerType aiva::utils::MetaStructUtils::GenerateFrom(DictStructPointerType const& dictStruct)
{
	Asserts::CheckBool(dictStruct, "Dict struct is not valid");

	auto const& structFields = dictStruct->FieldsBoxed();
	using StructFieldsIter = std::remove_reference_t<decltype(structFields)>::const_iterator;

	auto iterators = std::vector<StructFieldsIter>{};
	for (auto iterator = std::cbegin(structFields); iterator != std::cend(structFields); iterator++) iterators.push_back(iterator);
	std::sort(std::begin(iterators), std::end(iterators), [](auto const& a, auto const& b) { return a->first < b->first; });

	auto const metaStruct = NewObject<MetaStructElementType>();
	Asserts::CheckBool(metaStruct, "Meta struct is not valid");

	for (auto const& iterator : iterators)
	{
		auto const& fieldName = iterator->first;
		auto const& fieldValue = iterator->second;
		Asserts::CheckBool(fieldValue, "Field value is not valid");

		auto const metaField = NewObject<MetaFieldElementType>();
		Asserts::CheckBool(metaField, "Meta field is not valid");

		metaField->Name(fieldName);
		metaField->Type(fieldValue->Type());

		metaStruct->Add(metaField);
	}

	return metaStruct;
}

aiva::utils::MetaStructUtils::MetaStructPointerType aiva::utils::MetaStructUtils::GenerateFrom(LayoutStructPointerType const& layoutStruct)
{
	Asserts::CheckBool(layoutStruct, "Layout struct is not valid");

	auto const& layoutFields = layoutStruct->Fields();
	using LayoutFieldsIter = std::remove_reference_t<decltype(layoutFields)>::const_iterator;

	auto iterators = std::vector<LayoutFieldsIter>{};
	for (auto iterator = std::cbegin(layoutFields); iterator != std::cend(layoutFields); iterator++) iterators.push_back(iterator);
	std::sort(std::begin(iterators), std::end(iterators), [](auto const& a, auto const& b) { return a->first < b->first; });

	auto const metaStruct = NewObject<MetaStructElementType>();
	Asserts::CheckBool(metaStruct, "Meta struct is not valid");

	for (auto const& iterator : iterators)
	{
		auto const& fieldName = iterator->first;
		auto const& fieldValue = iterator->second;
		Asserts::CheckBool(fieldValue, "Field value is not valid");

		auto const metaField = NewObject<MetaFieldElementType>();
		Asserts::CheckBool(metaField, "Meta field is not valid");

		metaField->Name(fieldName);
		metaField->Type(fieldValue->Type());

		metaStruct->Add(metaField);
	}

	return metaStruct;
}
