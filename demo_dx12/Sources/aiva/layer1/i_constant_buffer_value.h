#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct IConstantBufferValue
	{
	public:
		virtual ~IConstantBufferValue() = default;
		virtual boost::span<const std::byte> GetRawData() const = 0;
	};
}
