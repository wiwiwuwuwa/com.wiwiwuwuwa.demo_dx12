#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct DictBuffer;
	struct LayoutBuffer;
	struct MetaBuffer;
}

namespace aiva::utils
{
	struct DictBufferUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		DictBufferUtils() = delete;

	// ----------------------------------------------------
	// Aliases

	public:
		using DictBufferElementType = DictBuffer;

		using DictBufferPointerType = std::shared_ptr<DictBufferElementType>;

		using LayoutBufferElementType = LayoutBuffer;

		using LayoutBufferPointerType = std::shared_ptr<LayoutBufferElementType>;

		using MetaBufferElementType = MetaBuffer;

		using MetaBufferPointerType = std::shared_ptr<MetaBufferElementType>;

	// ----------------------------------------------------
	// Generators

	public:
		static DictBufferPointerType GenerateFrom(LayoutBufferPointerType const& layoutBuffer);

		static DictBufferPointerType GenerateFrom(MetaBufferPointerType const& metaBuffer);

	// ----------------------------------------------------
	// Serialization

	public:
		static std::vector<std::byte> SerializeToBinary(DictBufferPointerType const& dictBuffer);

		static std::vector<std::byte> SerializeToBinary(DictBufferPointerType const& dictBuffer, MetaBufferPointerType const& metaBuffer);

		static std::vector<std::byte> SerializeToBinary(DictBufferPointerType const& dictBuffer, LayoutBufferPointerType const& layoutBuffer);

		static DictBufferPointerType DeserealizeFromBinary(MetaBufferPointerType const& metaBuffer, boost::span<const std::byte> const& bufferBinary);

		static DictBufferPointerType DeserealizeFromBinary(LayoutBufferPointerType const& metaBuffer, boost::span<const std::byte> const& bufferBinary);
	};
}