#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	struct logger_t final
	{
		// ------------------------------------------------

	private:
		logger_t() = delete;

		// ------------------------------------------------

	public:
		static void log(std::string const& message);

		// ------------------------------------------------
	};
}
