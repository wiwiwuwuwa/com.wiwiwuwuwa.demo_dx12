#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct LayoutField;
	struct LayoutStruct;
	struct DictStruct;
	struct MetaStruct;
}

namespace aiva::utils
{
	struct LayoutStructUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		LayoutStructUtils() = delete;

	// ----------------------------------------------------
	// Aliases

	public:
		using AlignFieldElementType = LayoutField;

		using AlignFieldPointerType = std::shared_ptr<AlignFieldElementType>;

		using AlignStructElementType = LayoutStruct;

		using AlignStructPointerType = std::shared_ptr<AlignStructElementType>;

		using DictStructElementType = DictStruct;

		using DictStructPointerType = std::shared_ptr<DictStructElementType>;

		using MetaStructElementType = MetaStruct;

		using MetaStructPointerType = std::shared_ptr<MetaStructElementType>;

	// ----------------------------------------------------
	// Comparison

	public:
		static bool IsMatchingLayout(AlignStructPointerType const& dstStruct, MetaStructPointerType const& srcLayout);

	// ----------------------------------------------------
	// Generators

	public:
		static AlignStructPointerType GenerateFrom(DictStructPointerType const& dictStruct);

		static AlignStructPointerType GenerateFrom(MetaStructPointerType const& metaStruct);
	};
}
