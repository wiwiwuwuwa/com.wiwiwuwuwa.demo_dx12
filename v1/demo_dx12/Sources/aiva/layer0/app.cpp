#include <pch.h>
#include <aiva/layer0/app.h>

namespace aiva::layer0
{
    using namespace aiva::utils;

    CoreWindow const& App::Window() const
    {
        return mWindow;
    }

    EvAction& App::OnStart()
    {
        return mOnStart;
    }

    EvAction& App::OnUpdate()
    {
        return mOnUpdate;
    }

    EvAction& App::OnFinish()
    {
        return mOnFinish;
    }

    IFrameworkView App::CreateView()
    {
        return *this;
    }

    void App::Initialize(CoreApplicationView const&)
    {
        mWindow = nullptr;
    }

    void App::Load(winrt::hstring const&)
    {

    }

    void App::Uninitialize()
    {
        mWindow = nullptr;
    }

    void App::Run()
    {
        auto window = CoreWindow::GetForCurrentThread();
        window.Activate();

        OnStart()();

        while (true)
        {
            auto dispatcher = window.Dispatcher();
            dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

            OnUpdate()();
        }

        OnFinish()();
    }

    void App::SetWindow(CoreWindow const& window)
    {
        mWindow = window;
    }
}
