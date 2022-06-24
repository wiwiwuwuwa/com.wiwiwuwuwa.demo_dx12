#pragma once

#include "pch.h"

namespace aiva
{
	struct Application : winrt::implements<Application, IFrameworkViewSource, IFrameworkView>
	{
        IFrameworkView CreateView();

        void Initialize(CoreApplicationView const&);

        void Load(winrt::hstring const&);

        void Uninitialize();

        void Run();

        void SetWindow(CoreWindow const&);
	};
}
