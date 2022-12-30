#pragma once
#include <pch.h>

#include <aiva2/engine/m_field_for_event.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/native/uwp/window_fwd.h>

namespace aiva2::native
{
    struct Window : winrt::implements<Window, IFrameworkViewSource, IFrameworkView>
    {
    // ----------------------------------------------------
    // Main

    public:
        using CoreWindowType = winrt::Windows::UI::Core::CoreWindow;

    public:
        M_STRUCT_BODY(Window);

    public:
        M_FIELD_FOR_EVENT_2(public, OnInit);

        M_FIELD_FOR_EVENT_2(public, OnTick);

        M_FIELD_FOR_EVENT_2(public, OnShut);

    // ----------------------------------------------------
    // IFrameworkViewSource

    public:
        auto CreateView() -> IFrameworkView;

    // ----------------------------------------------------
    // IFrameworkView

    public:
        void Initialize(CoreApplicationView const&);

        void Load(winrt::hstring const&);

        void Uninitialize();

        void Run();

        void SetWindow(CoreWindowType const& window);

    // ----------------------------------------------------
    // Window

    public:
        auto CoreWindow() const -> CoreWindowType const&;

    private:
        CoreWindowType mWindow{ nullptr };
    };
}
