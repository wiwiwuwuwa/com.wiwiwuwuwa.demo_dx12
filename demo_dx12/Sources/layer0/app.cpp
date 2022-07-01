#include <pch.h>
#include <layer0/app.h>

CoreWindow const& aiva::layer0::App::Window() const
{
    return mWindow;
}

IFrameworkView aiva::layer0::App::CreateView()
{
    return *this;
}

void aiva::layer0::App::Initialize(CoreApplicationView const&)
{
    mWindow = nullptr;
}

void aiva::layer0::App::Load(winrt::hstring const&)
{
}

void aiva::layer0::App::Uninitialize()
{
    mWindow = nullptr;
}

void aiva::layer0::App::Run()
{
    CoreWindow window = CoreWindow::GetForCurrentThread();
    window.Activate();

    while (true)
    {
        CoreDispatcher dispatcher = window.Dispatcher();
        dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
    }
}

void aiva::layer0::App::SetWindow(CoreWindow const& window)
{
    mWindow = window;
}
