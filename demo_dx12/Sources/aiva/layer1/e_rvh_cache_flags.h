#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class ERvhCacheFlags
	{
		None = 0,
		HeapPtr = 1 << 0,
		HeapBin = 1 << 1,
		All = HeapPtr | HeapBin,
	};
}
