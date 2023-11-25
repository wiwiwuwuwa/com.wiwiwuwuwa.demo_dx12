#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct LayoutField;
	struct MetaField;
}

namespace aiva::utils
{
	struct LayoutFieldUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		LayoutFieldUtils() = delete;

	// ----------------------------------------------------
	// Aliases

	public:
		using LayoutFieldElementType = LayoutField;

		using LayoutFieldPointerType = std::shared_ptr<LayoutFieldElementType>;

		using MetaFieldElementType = MetaField;

		using MetaFieldPointerType = std::shared_ptr<MetaFieldElementType>;

	// ----------------------------------------------------
	// Comparison

	public:
		static bool IsMatchingLayout(LayoutFieldPointerType const& dstField, MetaFieldPointerType const& srcField);
	};
}
