#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EGrvCacheFlags
	{
		None = 0,
		BufferPtr = 1 << 0,
		BufferBin = 1 << 1,
		All = BufferPtr | BufferBin,
	};
}
