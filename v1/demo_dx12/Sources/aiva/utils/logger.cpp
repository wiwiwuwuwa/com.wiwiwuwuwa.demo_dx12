#include <pch.h>
#include <aiva/utils/logger.h>

namespace aiva::utils
{
	void Logger::LogToDebugConsole(std::string const& message)
	{
		auto const aivaMessage = "aiva: " + message + "\n";
		OutputDebugStringA(aivaMessage.c_str());
	}
}
