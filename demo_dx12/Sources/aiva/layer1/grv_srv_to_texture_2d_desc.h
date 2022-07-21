#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GrTexture2D;
}

namespace aiva::layer1
{
	struct GrvSrvToTexture2DDesc final
	{
		// ----------------------------------------------------
		// Resource

	public:
		std::shared_ptr<GrTexture2D> Resource{};
	};
}
