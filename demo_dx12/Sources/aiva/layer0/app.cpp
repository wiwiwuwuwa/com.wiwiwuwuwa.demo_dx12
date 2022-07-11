#include <pch.h>
#include <aiva/layer0/app.h>

aiva::utils::EvAction& aiva::layer0::App::OnStart()
{
    return mOnStart;
}

aiva::utils::EvAction& aiva::layer0::App::OnUpdate()
{
    return mOnUpdate;
}

aiva::utils::EvAction& aiva::layer0::App::OnFinish()
{
    return mOnFinish;
}

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

    OnStart()();

    while (true)
    {
        CoreDispatcher dispatcher = window.Dispatcher();
        dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

        OnUpdate()();
    }

    OnFinish()();
}

void aiva::layer0::App::SetWindow(CoreWindow const& window)
{
    mWindow = window;
}
