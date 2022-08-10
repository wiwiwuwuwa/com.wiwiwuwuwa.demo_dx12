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
	struct MetaBufferUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		MetaBufferUtils() = delete;

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
		static MetaBufferPointerType GenerateFrom(DictBufferPointerType const& dictBuffer);

		static MetaBufferPointerType GenerateFrom(LayoutBufferPointerType const& layoutBuffer);
	};
}
