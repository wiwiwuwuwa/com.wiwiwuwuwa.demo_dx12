#pragma once
#include <pch.h>

namespace aiva::layer1_ext
{
	enum class RtResourceViewType
	{
		Unknown,
		Cbv,
		Srv,
		Uav,
		Sampler,
		Rtv,
		Dsv,
		Maximum,
	};
}
