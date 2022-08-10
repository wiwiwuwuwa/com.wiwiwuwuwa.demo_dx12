#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct AlignStructInfo;
	struct DictStruct;
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
		using AlignStructElementType = AlignStructInfo;

		using AlignStructPointerType = std::shared_ptr<AlignStructElementType>;

		using DictStructElementType = DictStruct;

		using DictStructPointerType = std::shared_ptr<DictStructElementType>;

	// ----------------------------------------------------
	// Comparison

	public:
		static bool HasSameFields(DictStructPointerType const& structA, DictStructPointerType const structB);

	// ----------------------------------------------------
	// Serialization

	public:
		static std::vector<std::byte> SerializeToBinary(DictStructPointerType const& dictStruct);

		static std::vector<std::byte> SerializeToBinary(DictStructPointerType const& dictStruct, AlignStructPointerType const& alignStruct);

		static DictStructPointerType DeserealizeFromBinary(AlignStructPointerType const& alignStruct, boost::span<const std::byte> const& structBinary);
	};
}
