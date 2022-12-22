#include <pch.h>
#include <aiva2/native/uwp/uwp_window.h>

namespace aiva2::native
{
    IFrameworkView UwpWindow::CreateView()
    {
        return *this;
    }

    void UwpWindow::Initialize(CoreApplicationView const&)
    {
        mWindow = nullptr;
    }

    void UwpWindow::Load(winrt::hstring const&)
    {

    }

    void UwpWindow::Uninitialize()
    {
        mWindow = nullptr;
    }

    void UwpWindow::Run()
    {
        auto UwpWindow = CoreWindow::GetForCurrentThread();
        UwpWindow.Activate();

        OnInit().Broadcast();

        while (true)
        {
            auto dispatcher = UwpWindow.Dispatcher();
            dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

            OnTick().Broadcast();
        }

        OnShut().Broadcast();
    }

    void UwpWindow::SetWindow(CoreWindow const& window)
    {
        mWindow = window;
    }

    CoreWindow const& UwpWindow::Window() const
    {
        return mWindow;
    }
}
