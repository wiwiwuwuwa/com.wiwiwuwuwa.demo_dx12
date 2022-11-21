#pragma once
#include <pch.h>

namespace aiva::layer1_ext
{
	enum class RtResourceHeapType
	{
		Unknown,
		CbvSrvUav,
		Sampler,
		Rtv,
		Dsv,
		Maximum,
	};
}
