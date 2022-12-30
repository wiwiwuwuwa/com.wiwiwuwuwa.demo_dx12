#pragma once
#include <pch.h>

#include <aiva2/engine/i_object.h>
#include <aiva2/engine/m_field_by_ref.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/engine/t_event_action.h>
#include <aiva2/native/engine_fwd.h>
#include <aiva2/native/uwp/window.h>
#include <aiva2/native/uwp/window_system_fwd.h>

namespace aiva2::native
{
    struct WindowSystem final : public engine::IObject
    {
    // ----------------------------------------------------
    // Main

    public:
        M_STRUCT_BODY(WindowSystem);

    public:
        WindowSystem(native::Engine& engine);

        ~WindowSystem() override;

    public:
        M_FIELD_BY_REF_3(public, native::Engine, Engine);

    // ----------------------------------------------------
    // Window

    public:
        void RunWindow() const;

        auto CoreWindow() const -> CoreWindow const&;

    public:
        auto OnWindowInit() const -> engine::EventActionReadOnly&;

        auto OnWindowTick() const -> engine::EventActionReadOnly&;

        auto OnWindowShut() const -> engine::EventActionReadOnly&;

    private:
        auto Window() const -> native::Window&;

    private:
        void InitWindow();

        void ShutWindow();

    private:
        winrt::com_ptr<native::Window> mWindow{};
    };
}
