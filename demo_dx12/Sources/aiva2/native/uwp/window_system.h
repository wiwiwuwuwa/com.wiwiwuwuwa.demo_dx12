#pragma once
#include <pch.h>

#include <aiva2/engine/m_field_for_event.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/engine/window_system.h>
#include <aiva2/native/uwp/engine.h>
#include <aiva2/native/uwp/uwp_window.h>
#include <aiva2/native/uwp/window_system_fwd.h>

namespace aiva2::native
{
    struct WindowSystem final : public engine::WindowSystem
    {
    // ----------------------------------------------------
    // Main

    public:
        M_STRUCT_BODY(WindowSystem);

    public:
        WindowSystem(native::Engine& engine);

        ~WindowSystem() override;

    // ----------------------------------------------------
    // Engine (Virtual)

    public:
        native::Engine& Engine() const override;

    private:
        native::Engine& mEngine;

    // ----------------------------------------------------
    // IObject

    public:
        void Init() override;

        void Shut() override;

    // ----------------------------------------------------
    // Window (Common)

    public:
        UwpWindow& Window() const;

    private:
        void InitWindow();

        void ShutWindow();

    private:
        void Window_When_Window_OnInit();

        void Window_When_Window_OnTick();

        void Window_When_Window_OnShut();

    private:
        winrt::com_ptr<UwpWindow> mWindow{};

    // ----------------------------------------------------
    // Window (Virtual)

    public:
        M_FIELD_FOR_EVENT_2(public, OnWindowInit);

        M_FIELD_FOR_EVENT_2(public, OnWindowTick);

        M_FIELD_FOR_EVENT_2(public, OnWindowShut);

    public:
        void RunWindow() override;
    };
}
