#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GrvSrvToBuffer;

	using GrvSrvToBufferType = GrvSrvToBuffer;

	using GrvSrvToBufferTypeShared = std::shared_ptr<GrvSrvToBufferType>;

	using GrvSrvToBufferTypeWeak = std::weak_ptr<GrvSrvToBufferType>;
}
