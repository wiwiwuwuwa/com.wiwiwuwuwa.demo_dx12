#pragma once
#include <string>

namespace aiva
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

    private:
        static void abort(std::string const& message = {});

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva
{
	template <typename t_val>
	void assert_t::check_bool(t_val const& value, std::string const& message /*= {}*/)
	{
        if (value)
        {
            return;
        }
        else
        {
            abort(message);
        }
	}

	template <typename t_val>
	void assert_t::check_hresult(t_val const& value, std::string const& message /*= {}*/)
	{
        if (SUCCEEDED(value))
        {
            return;
        }
        else
        {
            abort(message);
        }
	}
}
