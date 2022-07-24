#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GrBuffer;
	struct ShaderStruct;
}

namespace aiva::layer1
{
	struct GrvUavToBufferDesc final
	{
	// ----------------------------------------------------
	// Resource

	public:
		std::shared_ptr<GrBuffer> Resource{};

	// ----------------------------------------------------
	// Resource

	public:
		std::shared_ptr<const ShaderStruct> Struct{};
	};
}
