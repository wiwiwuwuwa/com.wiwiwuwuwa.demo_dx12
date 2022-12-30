#include <pch.h>
#include <aiva2/native/uwp/window.h>

namespace aiva2::native
{
    auto Window::CreateView() -> IFrameworkView
    {
        return *this;
    }

    void Window::Initialize(CoreApplicationView const&)
    {
        mWindow = nullptr;
    }

    void Window::Load(winrt::hstring const&)
    {

    }

    void Window::Uninitialize()
    {
        mWindow = nullptr;
    }

    void Window::Run()
    {
        auto Window = CoreWindow::GetForCurrentThread();
        Window.Activate();

        OnInit().Broadcast();

        while (true)
        {
            auto dispatcher = Window.Dispatcher();
            dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

            OnTick().Broadcast();
        }

        OnShut().Broadcast();
    }

    void Window::SetWindow(CoreWindowType const& window)
    {
        mWindow = window;
    }

    auto Window::CoreWindow() const -> CoreWindowType const&
    {
        return mWindow;
    }
}
