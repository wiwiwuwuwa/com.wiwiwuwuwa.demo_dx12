#include <pch.h>
#include <aiva2/native/uwp/engine.h>

#include <aiva2/engine/asserts.h>
#include <aiva2/engine/object_utils.h>

namespace aiva2::native
{
	using namespace engine;

	Engine::Engine()
	{

	}

	Engine::~Engine()
	{

	}

	native::WindowSystem& Engine::WindowSystem() const
	{
		Asserts::IsTrue(mWindowSystem, "Window system is not valid");
		return *mWindowSystem;
	}

	void Engine::InitWindowSystem()
	{
		mWindowSystem = NewObject<native::WindowSystem>(*this);
		Asserts::IsTrue(mWindowSystem, "Window system is not valid");
	}

	void Engine::ShutWindowSystem()
	{
		Asserts::IsTrue(mWindowSystem, "Window system is not valid");
		mWindowSystem = {};
	}

	native::GraphicHardware& Engine::GraphicHardware() const
	{
		Asserts::IsTrue(mGraphicHardware, "Graphic hardware is not valid");
		return *mGraphicHardware;
	}

	void Engine::InitGraphicHardware()
	{
		mGraphicHardware = NewObject<native::GraphicHardware>(*this);
		Asserts::IsTrue(mGraphicHardware, "Graphic hardware is not valid");
	}

	void Engine::ShutGraphicHardware()
	{
		Asserts::IsTrue(mGraphicHardware, "Graphic hardware is not valid");
		mGraphicHardware = {};
	}
}
