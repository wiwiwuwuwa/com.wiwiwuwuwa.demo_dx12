#include <pch.h>
#include <aiva2/native/uwp/window_system.h>

#include <aiva2/engine/asserts.h>

namespace aiva2::native
{
	using namespace engine;

	WindowSystem::WindowSystem(native::Engine& engine) : mEngine{ engine }
	{
		InitWindow();
	}

	WindowSystem::~WindowSystem()
	{
		ShutWindow();
	}

	void WindowSystem::RunWindow() const
	{
		Asserts::IsTrue(mWindow, "Window is not valid");
		CoreApplication::Run(Window());
	}

	CoreWindow const& WindowSystem::CoreWindow() const
	{
		return Window().CoreWindow();
	}

	auto WindowSystem::OnWindowInit() const -> engine::EventActionReadOnly&
	{
		return Window().OnInit();
	}

	auto WindowSystem::OnWindowTick() const -> engine::EventActionReadOnly&
	{
		return Window().OnTick();
	}

	auto WindowSystem::OnWindowShut() const -> engine::EventActionReadOnly&
	{
		return Window().OnShut();
	}

	auto WindowSystem::Window() const -> native::Window&
	{
		Asserts::IsTrue(mWindow, "Window is not valid");
		return *mWindow;
	}

	void WindowSystem::InitWindow()
	{
		mWindow = winrt::make_self<decltype(mWindow)::type>();
		Asserts::IsTrue(mWindow, "Window is not valid");
	}

	void WindowSystem::ShutWindow()
	{
		Asserts::IsTrue(mWindow, "Window is not valid");
		mWindow = {};
	}
}
