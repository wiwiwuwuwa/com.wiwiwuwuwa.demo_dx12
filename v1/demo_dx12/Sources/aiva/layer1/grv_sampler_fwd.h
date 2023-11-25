#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GrvSampler;

	using GrvSamplerType = GrvSampler;
	using GrvSamplerTypeShared = std::shared_ptr<GrvSamplerType>;
	using GrvSamplerTypeWeak = std::weak_ptr<GrvSamplerType>;
}
