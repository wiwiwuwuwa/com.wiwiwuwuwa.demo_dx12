#include <pch.h>
#include <aiva/utils/meta_field_utils.h>

#include <aiva/utils/asserts.h>
#include <aiva/utils/e_boxed_type.h>
#include <aiva/utils/meta_field.h>
#include <aiva/utils/object_utils.h>

aiva::utils::MetaFieldUtils::MetaFieldPointerType aiva::utils::MetaFieldUtils::Copy(MetaFieldPointerType const& metaField)
{
	Asserts::CheckBool(metaField, "Meta field is not valid");

	auto const metaFieldCopy = NewObject<MetaFieldElementType>();
	Asserts::CheckBool(metaFieldCopy, "Meta field copy is not valid");

	metaFieldCopy->Name(metaField->Name());
	metaFieldCopy->Type(metaField->Type());

	return metaFieldCopy;
}
