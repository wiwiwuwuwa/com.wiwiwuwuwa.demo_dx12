#pragma once
#include <pch.h>

namespace aiva::layer0
{
    struct App : winrt::implements<App, IFrameworkViewSource, IFrameworkView>
    {
    // ----------------------------------------------------
    // App

    public:
        CoreWindow const& Window() const;

    private:
        CoreWindow mWindow{ nullptr };

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
