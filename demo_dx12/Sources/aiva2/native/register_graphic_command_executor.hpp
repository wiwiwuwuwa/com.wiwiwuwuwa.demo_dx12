#pragma once
#include <aiva2/base.hpp>

#include <aiva2/native/graphic_command_executors.hpp>

#define REGISTER_GRAPHIC_COMMAND_EXECUTOR(command, executor) \
\
struct registration_for_##executor \
{ \
public: \
	registration_for_##executor() \
	{ \
		graphic_command_executors_t::register_executor<command, executor>(); \
	}; \
}; \
\
static registration_for_##executor m_registration_for_##executor{}; \
