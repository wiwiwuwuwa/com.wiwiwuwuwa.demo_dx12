#pragma once
#include <pch.h>

namespace aiva::layer1_ext
{
	enum class UtBarriersFlags
	{
		None,
		Setup = 1 << 0,
		Reset = 1 << 1,
	};
}
