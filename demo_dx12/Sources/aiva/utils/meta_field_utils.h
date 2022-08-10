#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct MetaField;
}

namespace aiva::utils
{
	struct MetaFieldUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		MetaFieldUtils() = delete;

	// ----------------------------------------------------
	// Aliases

	public:
		using MetaFieldElementType = MetaField;

		using MetaFieldPointerType = std::shared_ptr<MetaFieldElementType>;

	// ----------------------------------------------------
	// Copying

	public:
		static MetaFieldPointerType Copy(MetaFieldPointerType const& metaField);
	};
}
