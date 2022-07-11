#pragma once
#include <pch.h>

#include <stdexcept>

namespace aiva::utils
{
	struct Asserts final
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename T>
		static void CheckBool(T const& value);

	private:
		Asserts() = delete;
	};
}

// --------------------------------------------------------

template <typename T>
void aiva::utils::Asserts::CheckBool(T const& value)
{
	if (!value)
	{
		throw std::exception{};
	}
}
