#include <pch.h>
#include <aiva2/native/window_factory.h>

#include <aiva2/engine/i_window.h>
#include <aiva2/engine/object_utils.h>

#if defined(AIVA_PLATFORM_UWP)
#include <aiva2/native/uwp/window.h>
#endif

namespace aiva2::native
{
	using namespace engine;

	auto WindowFactory::Create() -> std::shared_ptr<IWindow>
	{
		return NewObject<Window>();
	}
}
