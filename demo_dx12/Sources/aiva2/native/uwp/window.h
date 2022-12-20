#pragma once
#include <pch.h>

#include <aiva2/engine/i_object.h>
#include <aiva2/engine/i_window.h>
#include <aiva2/engine/m_field_by_var.h>
#include <aiva2/engine/m_field_for_event.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/native/uwp/uwp_window.h>
#include <aiva2/native/uwp/window_fwd.h>

namespace aiva2::native
{
    struct Window final : public engine::IObject, public engine::IWindow
    {
    // ----------------------------------------------------
    // Main

    public:
        M_STRUCT_BODY(Window);

    public:
        Window();

        ~Window() override;

    // ----------------------------------------------------
    // NativeWindow

    public:
        M_FIELD_BY_VAR_3(private, winrt::com_ptr<UwpWindow>, NativeWindow);

    private:
        void InitNativeWindow();

        void ShutNativeWindow();

    private:
        void NativeWindow_OnInit();

        void NativeWindow_OnTick();

        void NativeWindow_OnShut();

    // ----------------------------------------------------
    // IWindow

    public:
        M_FIELD_FOR_EVENT_2(public, OnInit);

        M_FIELD_FOR_EVENT_2(public, OnTick);

        M_FIELD_FOR_EVENT_2(public, OnShut);

    public:
        void Run() override;
    };
}
