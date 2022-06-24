#include "pch.h"
#include "application.h"

#include "renderer.h"

IFrameworkView aiva::Application::CreateView()
{
    return *this;
}

void aiva::Application::Initialize(CoreApplicationView const&)
{
}

void aiva::Application::Load(winrt::hstring const&)
{
}

void aiva::Application::Uninitialize()
{
}

void aiva::Application::Run()
{
    CoreWindow window = CoreWindow::GetForCurrentThread();
    window.Activate();

    aiva::Renderer renderer{};

    CoreDispatcher dispatcher = window.Dispatcher();
    dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
}

void aiva::Application::SetWindow(CoreWindow const&)
{
}
