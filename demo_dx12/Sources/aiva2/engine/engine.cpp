#include <pch.h>
#include <aiva2/engine/engine.h>

#include <aiva2/engine/asserts.h>
#include <aiva2/engine/graphic_hardware.h>
#include <aiva2/engine/object_utils.h>
#include <aiva2/engine/time_system.h>
#include <aiva2/engine/window_system.h>

namespace aiva2::engine
{
	Engine::Engine()
	{
		InitSystems();
	}

	Engine::~Engine()
	{
		ShutSystems();
	}

	void Engine::Run()
	{
		WindowSystem().Run();
	}

	void Engine::InitSystems()
	{
		InitWindowSystem();
		InitTimeSystem();
		InitGraphicHardware();
	}

	void Engine::ShutSystems()
	{
		ShutGraphicHardware();
		ShutTimeSystem();
		ShutWindowSystem();
	}

	engine::WindowSystem& Engine::WindowSystem()
	{
		Asserts::IsTrue(mWindowSystem, "Window system is not valid");
		return *mWindowSystem;
	}

	void Engine::InitWindowSystem()
	{
		Asserts::IsTrue(mWindowSystem, "Window system is not valid");
		mWindowSystem = NewObject<decltype(mWindowSystem)::element_type>(*this);
	}

	void Engine::ShutWindowSystem()
	{
		Asserts::IsTrue(mWindowSystem, "Window system is not valid");
		mWindowSystem = {};
	}

	engine::TimeSystem& Engine::TimeSystem()
	{
		Asserts::IsTrue(mTimeSystem, "Time system is not valid");
		return *mTimeSystem;
	}

	void Engine::InitTimeSystem()
	{
		Asserts::IsTrue(mTimeSystem, "Time system is not valid");
		mTimeSystem = NewObject<decltype(mTimeSystem)::element_type>(*this);
	}

	void Engine::ShutTimeSystem()
	{
		Asserts::IsTrue(mTimeSystem, "Time system is not valid");
		mTimeSystem = {};
	}

	engine::GraphicHardware& Engine::GraphicHardware()
	{
		Asserts::IsTrue(mGraphicHardware, "Graphic hardware is not valid");
		return *mGraphicHardware;
	}

	void Engine::InitGraphicHardware()
	{
		Asserts::IsTrue(mGraphicHardware, "Graphic hardware is not valid");
		mGraphicHardware = NewObject<decltype(mGraphicHardware)::element_type>(*this);
	}

	void Engine::ShutGraphicHardware()
	{
		Asserts::IsTrue(mGraphicHardware, "Graphic hardware is not valid");
		mGraphicHardware = {};
	}
}
