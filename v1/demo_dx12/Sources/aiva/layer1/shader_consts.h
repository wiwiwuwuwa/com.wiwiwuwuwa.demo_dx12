#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct ShaderConsts final
	{
	// ----------------------------------------------------
	// Main

	private:
		ShaderConsts() = delete;

	// ----------------------------------------------------
	// Constants

	public:
		static constexpr std::size_t FLOAT_SIZE = sizeof(float);
		static constexpr std::size_t VECTOR_SIZE = FLOAT_SIZE * 4;
		static constexpr std::size_t HEAP_SIZE = VECTOR_SIZE * 16;
	};
}
