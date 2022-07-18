#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct ICpuResource
	{
	public:
		virtual ~ICpuResource() = default;
		virtual void Deserealize(std::vector<std::byte> const& binaryData) = 0;
	};
}
