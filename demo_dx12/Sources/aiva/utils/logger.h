#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct Logger final
	{
	// ----------------------------------------------------
	// Main

	private:
		Logger() = delete;

	// ----------------------------------------------------
	// Logger

	public:
		static void LogToDebugConsole(std::string const& message);
	};
}