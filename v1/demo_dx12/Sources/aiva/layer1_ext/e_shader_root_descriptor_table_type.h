#pragma once
#include <pch.h>

namespace aiva::layer1_ext
{
	enum class EShaderRootDescriptorTableType
	{
		Unknown,
		CbvSrvUav,
		Sampler,
		Max,
	};
}
