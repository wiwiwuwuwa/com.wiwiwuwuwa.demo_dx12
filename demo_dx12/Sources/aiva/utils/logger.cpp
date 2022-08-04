#include <pch.h>
#include <aiva/utils/logger.h>

void aiva::utils::Logger::LogToDebugConsole(std::string const& message)
{
	auto const aivaMessage = "aiva: " + message + "\n";
	OutputDebugStringA(aivaMessage.c_str());
}
