#pragma once
#include <pch.h>

namespace aiva::utils_ext
{
	enum class EValueType
	{
		Unknown,
		UInt8,
		UInt16,
		UInt32,
		UInt32x2,
		UInt32x3,
		UInt32x4,
		UInt32x4x4,
		Float32,
		Float32x2,
		Float32x3,
		Float32x4,
		Float32x4x4,
		Maximum,
	};
}
