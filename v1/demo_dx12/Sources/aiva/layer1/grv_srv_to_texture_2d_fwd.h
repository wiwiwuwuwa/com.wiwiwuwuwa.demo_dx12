#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GrvSrvToTexture2D;

	using GrvSrvToTexture2DType = GrvSrvToTexture2D;

	using GrvSrvToTexture2DShared = std::shared_ptr<GrvSrvToTexture2DType>;

	using GrvSrvToTexture2DWeak = std::weak_ptr<GrvSrvToTexture2DType>;
}
