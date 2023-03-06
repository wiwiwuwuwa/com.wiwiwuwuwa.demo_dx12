#include <pch.h>
#include <aiva2/logger.hpp>

namespace aiva2
{
	void logger_t::log(std::string const& message)
	{
		OutputDebugStringA(message.c_str());
	}
}
