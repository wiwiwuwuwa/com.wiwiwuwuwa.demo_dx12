#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GrBuffer;
}

namespace aiva::layer1
{
	struct GroDictionaryToBufferDesc final
	{
	// ----------------------------------------------------
	// Resource

	public:
		std::shared_ptr<GrBuffer> Resource{};
	};
}
