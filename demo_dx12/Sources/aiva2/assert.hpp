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

#include <aiva2/logger.hpp>

namespace aiva2
{
	template <typename t_val>
	void assert_t::check_bool(t_val const& value, std::string const& message /*= {}*/)
	{
		auto const is_valid = static_cast<bool>(value);
		if (!is_valid)
		{
			logger_t::log("aiva > " + message + "\n");
			winrt::check_bool(false);
		}
	}

	template <typename t_val>
	void assert_t::check_hresult(t_val const& value, std::string const& message /*= {}*/)
	{
		auto const is_valid = SUCCEEDED(value);
		if (!is_valid)
		{
			logger_t::log("aiva > " + message + "\n");
			winrt::check_hresult(value);
		}
	}
}
