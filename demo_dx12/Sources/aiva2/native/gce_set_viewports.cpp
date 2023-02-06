#include <pch.h>
#include <aiva2/native/gce_set_viewports.hpp>

#include <aiva2/native/gcd_set_viewports.hpp>
#include <aiva2/native/register_graphic_command_executor.hpp>

namespace aiva2::native
{
	REGISTER_GRAPHIC_COMMAND_EXECUTOR(gcd_set_viewports_t, gce_set_viewports_t);
}
