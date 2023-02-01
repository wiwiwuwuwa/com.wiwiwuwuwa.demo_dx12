#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>

namespace aiva2::native
{
    struct window_t : winrt::implements<window_t, IFrameworkViewSource, IFrameworkView>, public core::implements_t<window_t>
    {
        // ------------------------------------------------
        // types

    public:
        using core_window_type = winrt::Windows::UI::Core::CoreWindow;

        using this_type = window_t;

        // ------------------------------------------------
        // events

    public:
        auto get_on_init() -> event_action_type<>&;

        auto get_on_tick() -> event_action_type<>&;

        auto get_on_shut() -> event_action_type<>&;

    private:
        event_action_type<> m_on_init{};

        event_action_type<> m_on_tick{};

        event_action_type<> m_on_shut{};

        // ------------------------------------------------
        // core window

    public:
        auto get_core_window() const -> core_window_type const&;

    private:
        core_window_type m_core_window{ nullptr };

        // ------------------------------------------------
        // IFrameworkViewSource

    public:
        auto CreateView() -> IFrameworkView;

        // ------------------------------------------------
        // IFrameworkView

    public:
        void Initialize(CoreApplicationView const&);

        void Load(winrt::hstring const&);

        void Uninitialize();

        void Run();

        void SetWindow(core_window_type const& core_window);

        // ------------------------------------------------
    };
}
