#pragma once
#include <pch.h>

#include <winrt/base.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer0
{
    struct App : winrt::implements<App, IFrameworkViewSource, IFrameworkView>
    {
    // ----------------------------------------------------
    // App Data

    public:
        CoreWindow const& Window() const;

    private:
        CoreWindow mWindow{ nullptr };

    // ----------------------------------------------------
    // App Events

    public:
        aiva::utils::EvAction& OnStart();

        aiva::utils::EvAction& OnUpdate();

        aiva::utils::EvAction& OnFinish();

    private:
        aiva::utils::EvAction mOnStart{};

        aiva::utils::EvAction mOnUpdate{};

        aiva::utils::EvAction mOnFinish{};

    // ----------------------------------------------------
    // IFrameworkViewSource

    public:
        IFrameworkView CreateView();

    // ----------------------------------------------------
    // IFrameworkView

    public:
        void Initialize(CoreApplicationView const&);

        void Load(winrt::hstring const&);

        void Uninitialize();

        void Run();

        void SetWindow(CoreWindow const&);
    };
}
