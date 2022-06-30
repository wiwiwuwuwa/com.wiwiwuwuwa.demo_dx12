#include <pch.h>
#include <core/engine.h>
#include <core/renderer.h>

CoreWindow const& aiva::Engine::GetWindow() const
{
    return mWindow;
}

IFrameworkView aiva::Engine::CreateView()
{
    return *this;
}

void aiva::Engine::Initialize(CoreApplicationView const&)
{
}

void aiva::Engine::Load(winrt::hstring const&)
{
}

void aiva::Engine::Uninitialize()
{
}

void aiva::Engine::Run()
{
    CoreWindow window = CoreWindow::GetForCurrentThread();
    window.Activate();

    aiva::Renderer renderer{ get_strong() };

    CoreDispatcher dispatcher = window.Dispatcher();
    dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
}

void aiva::Engine::SetWindow(CoreWindow const& window)
{
    mWindow = window;
}
