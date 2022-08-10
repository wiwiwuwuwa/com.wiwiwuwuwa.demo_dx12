#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct DictStruct;
	struct MetaField;
	struct MetaStruct;
}

namespace aiva::utils
{
	struct MetaStructUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		MetaStructUtils() = delete;

	// ----------------------------------------------------
	// Aliases

	public:
		using DictStructElementType = DictStruct;

		using DictStructPointerType = std::shared_ptr<DictStructElementType>;

		using MetaStructElementType = MetaStruct;

		using MetaStructPointerType = std::shared_ptr<MetaStructElementType>;

		using MetaFieldElementType = MetaField;

		using MetaFieldPointerType = std::shared_ptr<MetaFieldElementType>;

	// ----------------------------------------------------
	// Generators

	public:
		static MetaStructPointerType GenerateFrom(DictStructPointerType const& dictStruct);
	};
}
