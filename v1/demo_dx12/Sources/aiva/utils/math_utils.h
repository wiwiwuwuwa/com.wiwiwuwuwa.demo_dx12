#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct MathUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		MathUtils() = delete;

	// ----------------------------------------------------
	// Math

	public:
		template <typename T>
		static T Align(T const& pointer, T const& alignment);

		template <typename T>
		static T Align(T const& pointer, T const& count, T const& alignment);

	// ----------------------------------------------------
	// Matrices

	public:
		static glm::mat4 const& BlenderToDirectxMatrix();
	};
}

// --------------------------------------------------------

template <typename T>
T aiva::utils::MathUtils::Align(T const& pointer, T const& alignment)
{
	auto const needAlign = ((pointer % alignment) != 0);
	return (needAlign ? ((pointer / alignment + 1) * alignment) : pointer);
}

template <typename T>
T aiva::utils::MathUtils::Align(T const& pointer, T const& size, T const& alignment)
{
	if (pointer % alignment == 0)
	{
		return pointer;
	}

	if (pointer / alignment == (pointer + size - 1) / alignment)
	{
		return pointer;
	}

	return (pointer / alignment + 1) * alignment;
}
