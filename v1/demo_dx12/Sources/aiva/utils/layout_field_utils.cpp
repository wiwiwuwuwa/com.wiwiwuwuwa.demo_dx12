#include <pch.h>
#include <aiva/utils/layout_field_utils.h>

#include <aiva/utils/layout_field.h>
#include <aiva/utils/meta_field.h>

bool aiva::utils::LayoutFieldUtils::IsMatchingLayout(LayoutFieldPointerType const& dstField, MetaFieldPointerType const& srcField)
{
	if (!dstField || !srcField)
	{
		return false;
	}

	if (dstField->Type() != srcField->Type())
	{
		return false;
	}

	return true;
}
