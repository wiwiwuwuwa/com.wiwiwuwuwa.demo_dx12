#pragma once
#include <pch.h>

#include <aiva2/engine/m_field_for_event.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/native/uwp/uwp_window_fwd.h>

namespace aiva2::native
{
    struct UwpWindow : winrt::implements<UwpWindow, IFrameworkViewSource, IFrameworkView>
    {
    // ----------------------------------------------------
    // Main

    public:
        M_STRUCT_BODY(UwpWindow);

    // ----------------------------------------------------

    public:
        M_FIELD_FOR_EVENT_2(public, OnInit);

        M_FIELD_FOR_EVENT_2(public, OnTick);

        M_FIELD_FOR_EVENT_2(public, OnShut);

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

        void SetWindow(CoreWindow const& window);

    // ----------------------------------------------------
    // Window

    public:
        CoreWindow const& Window() const;

    private:
        CoreWindow mWindow{ nullptr };
    };
}
