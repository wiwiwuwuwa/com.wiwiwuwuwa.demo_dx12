#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct AlignFieldInfo;
	struct AlignStructInfo;
	struct DictStruct;
	struct MetaStruct;
}

namespace aiva::utils
{
	struct AlignStructUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		AlignStructUtils() = delete;

	// ----------------------------------------------------
	// Aliases

	public:
		using AlignFieldElementType = AlignFieldInfo;

		using AlignFieldPointerType = std::shared_ptr<AlignFieldElementType>;

		using AlignStructElementType = AlignStructInfo;

		using AlignStructPointerType = std::shared_ptr<AlignStructElementType>;

		using DictStructElementType = DictStruct;

		using DictStructPointerType = std::shared_ptr<DictStructElementType>;

		using MetaStructElementType = MetaStruct;

		using MetaStructPointerType = std::shared_ptr<MetaStructElementType>;

	// ----------------------------------------------------
	// Generators

	public:
		static AlignStructPointerType GenerateFrom(DictStructPointerType const& dictStruct);

		static AlignStructPointerType GenerateFrom(MetaStructPointerType const& metaStruct);
	};
}
