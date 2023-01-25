#pragma once
#include <pch.h>

namespace aiva2::core
{
	struct asserts_t final
	{
		// ------------------------------------------------

	private:
		asserts_t() = delete;

		// ------------------------------------------------

	public:
		template <typename t_val>
		static void check_true(t_val const& value, std::string const& message = {});

		template <typename t_val>
		static void check_false(t_val const& value, std::string const& message = {});

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2::core
{
	template <typename t_val>
	void check_true(t_val const& value, std::string const& message = {})
	{
		if (!value)
		{
			throw std::runtime_error{ std::string{ "aiva: "} + message + std::string{ ";" } };
		}
	}

	template <typename t_val>
	void check_false(t_val const& value, std::string const& message = {})
	{
		check_true(!value, message);
	}
}
