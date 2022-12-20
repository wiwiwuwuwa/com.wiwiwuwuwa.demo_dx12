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

		mNativeWindow->OnInit().AttachListener(&ThisType::NativeWindow_OnInit, this);
		mNativeWindow->OnTick().AttachListener(&ThisType::NativeWindow_OnTick, this);
		mNativeWindow->OnShut().AttachListener(&ThisType::NativeWindow_OnShut, this);
	}

	void Window::ShutNativeWindow()
	{
		Asserts::IsTrue(mNativeWindow, "Native window is not valid");
		mNativeWindow->OnShut().RemoveListener(&ThisType::NativeWindow_OnShut, this);
		mNativeWindow->OnTick().RemoveListener(&ThisType::NativeWindow_OnTick, this);
		mNativeWindow->OnInit().RemoveListener(&ThisType::NativeWindow_OnInit, this);

		mNativeWindow = {};
	}

	void Window::NativeWindow_OnInit()
	{
		OnInit().Broadcast();
	}

	void Window::NativeWindow_OnTick()
	{
		OnTick().Broadcast();
	}

	void Window::NativeWindow_OnShut()
	{
		OnShut().Broadcast();
	}

	void Window::Run()
	{
		Asserts::IsTrue(mNativeWindow, "Native window is not valid");
		CoreApplication::Run(*mNativeWindow);
	}
}
