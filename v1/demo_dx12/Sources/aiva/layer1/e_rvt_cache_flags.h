#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class ERvtCacheFlags
	{
		None = 0,
		TablePtr = 1 << 0,
		TableBin = 1 << 1,
		All = TablePtr | TableBin,
	};
}
