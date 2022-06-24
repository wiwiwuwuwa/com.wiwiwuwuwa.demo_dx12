#include "pch.h"
#include "engine.h"

#include "renderer.h"

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

    aiva::Renderer renderer{};

    CoreDispatcher dispatcher = window.Dispatcher();
    dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
}

void aiva::Engine::SetWindow(CoreWindow const&)
{
}
