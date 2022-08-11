#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GrvDsvToTexture2D;

	using GrvDsvToTexture2DType = GrvDsvToTexture2D;
	using GrvDsvToTexture2DTypeShared = std::shared_ptr<GrvDsvToTexture2DType>;
	using GrvDsvToTexture2DTypeWeak = std::weak_ptr<GrvDsvToTexture2D>;
}
