#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct LayoutStruct;
	struct DictStruct;
	struct MetaStruct;
}

namespace aiva::utils
{
	struct DictStructUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		DictStructUtils() = delete;

	// ----------------------------------------------------
	// Aliases

	public:
		using AlignStructElementType = LayoutStruct;

		using AlignStructPointerType = std::shared_ptr<AlignStructElementType>;

		using DictStructElementType = DictStruct;

		using DictStructPointerType = std::shared_ptr<DictStructElementType>;

		using MetaStructElementType = MetaStruct;

		using MetaStructPointerType = std::shared_ptr<MetaStructElementType>;

	// ----------------------------------------------------
	// Comparison

	public:
		static bool HasSameFields(DictStructPointerType const& structA, DictStructPointerType const structB);

		static bool IsMatchingLayout(DictStructPointerType const& dstStruct, MetaStructPointerType const& srcLayout);

	// ----------------------------------------------------
	// Generators

	public:
		static DictStructPointerType GenerateFrom(AlignStructPointerType const& layoutStruct);

		static DictStructPointerType GenerateFrom(MetaStructPointerType const& metaStruct);

	// ----------------------------------------------------
	// Serialization

	public:
		static std::vector<std::byte> SerializeToBinary(DictStructPointerType const& dictStruct);

		static std::vector<std::byte> SerializeToBinary(DictStructPointerType const& dictStruct, MetaStructPointerType const& metaStruct);

		static std::vector<std::byte> SerializeToBinary(DictStructPointerType const& dictStruct, AlignStructPointerType const& alignStruct);

		static DictStructPointerType DeserealizeFromBinary(MetaStructPointerType const& metaStruct, boost::span<const std::byte> const& structBinary);

		static DictStructPointerType DeserealizeFromBinary(AlignStructPointerType const& alignStruct, boost::span<const std::byte> const& structBinary);
	};
}
