#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct IConstantBufferValue
	{
	public:
		virtual ~IConstantBufferValue() = default;
		virtual std::vector<std::byte> const& GetRawData() const = 0;
	};
}
