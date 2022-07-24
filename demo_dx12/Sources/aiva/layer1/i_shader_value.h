#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct IShaderValue
	{
	public:
		virtual ~IShaderValue() = default;

		virtual std::vector<std::byte> SerializeToBinary() const = 0;
	};
}
