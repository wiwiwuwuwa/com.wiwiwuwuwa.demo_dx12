#pragma once
#include <pch.h>

namespace aiva::layer1_ext
{
	enum class EShaderRegisterType
	{
		Unknown,
		CBV,
		SRV,
		UAV,
		Sampler,
		Max,
	};
}
