#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GrBuffer;

	using GrBufferType = GrBuffer;

	using GrBufferTypeShared = std::shared_ptr<GrBufferType>;

	using GrBufferTypeWeak = std::weak_ptr<GrBufferType>;
}
