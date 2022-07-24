#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GrBuffer;
	struct ShaderStruct;
}

namespace aiva::layer1
{
	struct GroStructToBufferDesc final
	{
	// ----------------------------------------------------
	// Resource

	public:
		std::shared_ptr<GrBuffer> Resource{};
	};
}
