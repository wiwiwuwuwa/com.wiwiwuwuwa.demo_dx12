#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GrvRtvToTexture2D;

	using GrvRtvToTexture2DType = GrvRtvToTexture2D;
	using GrvRtvToTexture2DTypeShared = std::shared_ptr<GrvRtvToTexture2DType>;
	using GrvRtvToTexture2DTypeWeak = std::weak_ptr<GrvRtvToTexture2DType>;
}
