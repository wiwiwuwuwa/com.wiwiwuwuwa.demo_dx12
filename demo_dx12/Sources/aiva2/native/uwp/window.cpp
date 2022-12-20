#include <pch.h>
#include <aiva2/native/uwp/window.h>

#include <aiva2/engine/asserts.h>

namespace aiva2::native
{
	using namespace engine;

	Window::Window()
	{
		InitNativeWindow();
	}

	Window::~Window()
	{
		ShutNativeWindow();
	}

	void Window::InitNativeWindow()
	{
		mNativeWindow = winrt::make_self<decltype(mNativeWindow)::type>();
		Asserts::IsTrue(mNativeWindow, "Native window is not valid");

		mNativeWindow->OnInit().AttachListener(&ThisType::NativeWindow_When_Window_OnInit, this);
		mNativeWindow->OnTick().AttachListener(&ThisType::NativeWindow_When_Window_OnTick, this);
		mNativeWindow->OnShut().AttachListener(&ThisType::NativeWindow_When_Window_OnShut, this);
	}

	void Window::ShutNativeWindow()
	{
		Asserts::IsTrue(mNativeWindow, "Native window is not valid");
		mNativeWindow->OnShut().RemoveListener(&ThisType::NativeWindow_When_Window_OnShut, this);
		mNativeWindow->OnTick().RemoveListener(&ThisType::NativeWindow_When_Window_OnTick, this);
		mNativeWindow->OnInit().RemoveListener(&ThisType::NativeWindow_When_Window_OnInit, this);

		mNativeWindow = {};
	}

	void Window::NativeWindow_When_Window_OnInit()
	{
		OnInit().Broadcast();
	}

	void Window::NativeWindow_When_Window_OnTick()
	{
		OnTick().Broadcast();
	}

	void Window::NativeWindow_When_Window_OnShut()
	{
		OnShut().Broadcast();
	}

	void Window::Run()
	{
		Asserts::IsTrue(mNativeWindow, "Native window is not valid");
		CoreApplication::Run(*mNativeWindow);
	}
}
