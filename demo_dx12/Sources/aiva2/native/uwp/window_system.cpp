#include <pch.h>
#include <aiva2/native/uwp/window_system.h>

#include <aiva2/engine/asserts.h>

namespace aiva2::native
{
	using namespace engine;

	WindowSystem::WindowSystem(native::Engine& engine) : mEngine{ engine }
	{

	}

	WindowSystem::~WindowSystem()
	{

	}

	native::Engine& WindowSystem::Engine() const
	{
		return mEngine;
	}

	void WindowSystem::Init()
	{
		engine::WindowSystem::Init();
		InitWindow();
	}

	void WindowSystem::Shut()
	{
		ShutWindow();
		engine::WindowSystem::Shut();
	}

	UwpWindow& WindowSystem::Window() const
	{
		Asserts::IsTrue(mWindow, "Window is not valid");
		return *mWindow;
	}

	void WindowSystem::InitWindow()
	{
		mWindow = winrt::make_self<decltype(mWindow)::type>();
		Asserts::IsTrue(mWindow, "Window is not valid");

		mWindow->OnInit().AttachListener(&ThisType::Window_When_Window_OnInit, this);
		mWindow->OnTick().AttachListener(&ThisType::Window_When_Window_OnTick, this);
		mWindow->OnShut().AttachListener(&ThisType::Window_When_Window_OnShut, this);
	}

	void WindowSystem::ShutWindow()
	{
		Asserts::IsTrue(mWindow, "Window is not valid");
		mWindow->OnShut().RemoveListener(&ThisType::Window_When_Window_OnShut, this);
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

	void WindowSystem::Window_When_Window_OnShut()
	{
		OnWindowShut().Broadcast();
	}

	void WindowSystem::RunWindow()
	{
		Asserts::IsTrue(mWindow, "Window is not valid");
		CoreApplication::Run(*mWindow);
	}
}
