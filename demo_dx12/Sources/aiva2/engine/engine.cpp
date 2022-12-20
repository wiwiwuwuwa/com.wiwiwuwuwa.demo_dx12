#include <pch.h>
#include <aiva2/engine/engine.h>

#include <aiva2/engine/asserts.h>
#include <aiva2/engine/i_window.h>
#include <aiva2/native/window_factory.h>

namespace aiva2::engine
{
	using namespace native;

	Engine::Engine()
	{
		InitWindow();
	}

	Engine::~Engine()
	{
		ShutWindow();
	}

	void Engine::Run()
	{
		Asserts::IsTrue(mWindow, "Window is not valid");
		mWindow->Run();
	}

	void Engine::InitWindow()
	{
		mWindow = WindowFactory::Create();
		Asserts::IsTrue(mWindow, "Window is not valid");

		mWindow->OnInit().AttachListener(&ThisType::Window_OnInit, this);
		mWindow->OnTick().AttachListener(&ThisType::Window_OnTick, this);
		mWindow->OnShut().AttachListener(&ThisType::Window_OnShut, this);
	}

	void Engine::ShutWindow()
	{
		Asserts::IsTrue(mWindow, "Window is not valid");
		mWindow->OnShut().AttachListener(&ThisType::Window_OnShut, this);
		mWindow->OnTick().AttachListener(&ThisType::Window_OnTick, this);
		mWindow->OnInit().AttachListener(&ThisType::Window_OnInit, this);

		mWindow = {};
	}

	void Engine::Window_OnInit()
	{
		OnInit().Broadcast();
	}

	void Engine::Window_OnTick()
	{
		OnTick().Broadcast();
		OnDraw().Broadcast();
	}

	void Engine::Window_OnShut()
	{
		OnShut().Broadcast();
	}
}
