#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	struct assert_t final
	{
		// ------------------------------------------------

	private:
		assert_t() = delete;

		// ------------------------------------------------

	public:
		template <typename t_val>
		static void check_bool(t_val const& value, std::string const& message = {});

		template <typename t_val>
		static void check_hresult(t_val const& value, std::string const& message = {});

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2
{
	template <typename t_val>
	void assert_t::check_bool(t_val const& value, std::string const& message /*= {}*/)
	{
		winrt::check_bool(static_cast<bool>(value));
	}

	template <typename t_val>
	void assert_t::check_hresult(t_val const& value, std::string const& message /*= {}*/)
	{
		winrt::check_hresult(value);
	}
}
