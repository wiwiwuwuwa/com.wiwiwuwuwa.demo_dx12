#pragma once

#include <pch.h>

namespace aiva
{
	struct Engine : winrt::implements<Engine, IFrameworkViewSource, IFrameworkView>
	{
    // ----------------------------------------------------
    // Engine

    public:
        CoreWindow const& GetWindow() const;

    private:
        CoreWindow mWindow{ nullptr };

    // ----------------------------------------------------
    // WinRT

    public:
        IFrameworkView CreateView();

        void Initialize(CoreApplicationView const&);

        void Load(winrt::hstring const&);

        void Uninitialize();

        void Run();

        void SetWindow(CoreWindow const&);
	};
}
