#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct IShaderValue
	{
	public:
		virtual ~IShaderValue() = default;

	// ----------------------------------------------------
	// Actions

	public:
		virtual std::vector<std::byte> SerializeToBinary() const = 0;

		virtual void DeserealizeFromBinary(boost::span<const std::byte> const& binary) = 0;

	// ----------------------------------------------------
	// Metadata

	public:
		virtual std::size_t MinBinarySize() const = 0;
	};
}
