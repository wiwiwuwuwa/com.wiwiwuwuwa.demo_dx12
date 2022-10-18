#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct Asserts final
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename T>
		static void CheckBool(T const& value, std::string const& message = {});

	private:
		Asserts() = delete;
	};
}

// --------------------------------------------------------

namespace aiva::utils
{
	template <typename T>
	void Asserts::CheckBool(T const& value, std::string const& message /*= {}*/)
	{
		if (!value)
		{
			throw std::runtime_error{ std::string{"aiva: "} + message };
		}
	}
}
