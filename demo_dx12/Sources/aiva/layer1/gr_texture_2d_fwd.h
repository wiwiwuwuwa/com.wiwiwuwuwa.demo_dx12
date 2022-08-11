#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GrTexture2D;

	using GrTexture2DType = GrTexture2D;
	using GrTexture2DTypeShared = std::shared_ptr<GrTexture2DType>;
	using GrTexture2DTypeWeak = std::weak_ptr<GrTexture2DType>;
}
