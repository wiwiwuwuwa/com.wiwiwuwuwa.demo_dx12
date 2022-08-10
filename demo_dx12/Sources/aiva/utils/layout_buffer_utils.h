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
	struct LayoutBufferUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		LayoutBufferUtils() = delete;

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
		static LayoutBufferPointerType GenerateFrom(DictBufferPointerType const& dictBuffer);

		static LayoutBufferPointerType GenerateFrom(MetaBufferPointerType const& metaBuffer);
	};
}