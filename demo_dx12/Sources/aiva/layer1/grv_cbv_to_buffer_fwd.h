#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GrvCbvToBuffer;

	using GrvCbvToBufferType = GrvCbvToBuffer;
	
	using GrvCbvToBufferTypeShared = std::shared_ptr<GrvCbvToBufferType>;

	using GrvCbvToBufferTypeWeak = std::weak_ptr<GrvCbvToBufferType>;
}
