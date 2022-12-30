#include <pch.h>
#include <aiva2/native/engine.h>

#include <aiva2/engine/asserts.h>
#include <aiva2/engine/object_utils.h>
#include <aiva2/native/graphic_hardware.h>
#include <aiva2/native/window_system.h>

namespace aiva2::native
{
	using namespace engine;

	Engine::Engine()
	{
		InitSystems();
	}

	Engine::~Engine()
	{
		ShutSystems();
	}

	void Engine::Run() const
	{
		WindowSystem().RunWindow();
	}

	void Engine::InitSystems()
	{
		InitWindowSystem();
		WindowSystem().OnWindowInit().AttachListener(&ThisType::Systems_When_WindowSystem_OnWindowInit, this);
	}

	void Engine::ShutSystems()
	{
		WindowSystem().OnWindowInit().RemoveListener(&ThisType::Systems_When_WindowSystem_OnWindowInit, this);
		ShutWindowSystem();
	}

	void Engine::Systems_When_WindowSystem_OnWindowInit()
	{
		InitGraphicHardware();
	}

	void Engine::Systems_When_WindowSystem_OnWindowShut()
	{
		ShutGraphicHardware();
	}

	auto Engine::WindowSystem() const -> native::WindowSystem&
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

	auto Engine::GraphicHardware() const -> native::GraphicHardware&
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
