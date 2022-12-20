#pragma once
#include <pch.h>

namespace aiva2::engine
{
	struct Asserts final
	{
	// ----------------------------------------------------

	private:
		Asserts() = delete;

	// ----------------------------------------------------

	public:
		template <typename T>
		static void IsTrue(T const& value, std::string const& message = {});

		template <typename T>
		static void IsFalse(T const& value, std::string const& message = {});
	};
}

// --------------------------------------------------------

namespace aiva2::engine
{
	template <typename T>
	void Asserts::IsTrue(T const& value, std::string const& message /*= {}*/)
	{
		if (!value)
		{
			throw std::runtime_error{ std::string{"aiva: "} + message };
		}
	}

	template <typename T>
	void Asserts::IsFalse(T const& value, std::string const& message /*= {}*/)
	{
		if (value)
		{
			throw std::runtime_error{ std::string{"aiva: "} + message };
		}
	}
}
