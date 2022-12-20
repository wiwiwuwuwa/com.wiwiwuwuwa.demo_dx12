#include <pch.h>
#include <aiva2/engine/window_system.h>

#include <aiva2/engine/asserts.h>
#include <aiva2/engine/engine.h>
#include <aiva2/engine/i_window.h>
#include <aiva2/native/window_factory.h>

namespace aiva2::engine
{
	using namespace native;

	WindowSystem::WindowSystem(engine::Engine& engine) : mEngine{ engine }
	{
		InitWindow();
	}

	WindowSystem::~WindowSystem()
	{
		ShutWindow();
	}

	void WindowSystem::Run()
	{
		Asserts::IsTrue(mWindow, "Window is not valid");
		mWindow->Run();
	}

	void WindowSystem::InitWindow()
	{
		mWindow = WindowFactory::Create();
		Asserts::IsTrue(mWindow, "Window is not valid");

		mWindow->OnInit().AttachListener(&ThisType::Window_When_Window_OnInit, this);
		mWindow->OnTick().AttachListener(&ThisType::Window_When_Window_OnTick, this);
		mWindow->OnShut().AttachListener(&ThisType::WIndow_When_Window_OnShut, this);
	}

	void WindowSystem::ShutWindow()
	{
		Asserts::IsTrue(mWindow, "Window is not valid");
		mWindow->OnShut().RemoveListener(&ThisType::WIndow_When_Window_OnShut, this);
		mWindow->OnTick().RemoveListener(&ThisType::Window_When_Window_OnTick, this);
		mWindow->OnInit().RemoveListener(&ThisType::Window_When_Window_OnInit, this);

		mWindow = {};
	}

	void WindowSystem::Window_When_Window_OnInit()
	{
		OnWindowInit().Broadcast();
	}

	void WindowSystem::Window_When_Window_OnTick()
	{
		OnWindowTick().Broadcast();
	}

	void WindowSystem::WIndow_When_Window_OnShut()
	{
		OnWindowShut().Broadcast();
	}
}
