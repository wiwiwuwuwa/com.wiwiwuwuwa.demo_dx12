#pragma once
#include <pch.h>

#include <aiva2/engine/engine_fwd.h>
#include <aiva2/engine/i_object.h>
#include <aiva2/engine/i_window_fwd.h>
#include <aiva2/engine/m_field_by_ref.h>
#include <aiva2/engine/m_field_by_var.h>
#include <aiva2/engine/m_field_for_event.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/engine/window_system_fwd.h>

namespace aiva2::engine
{
	struct WindowSystem final : public IObject
	{
	// ----------------------------------------------------
	// Main

	public:
		M_STRUCT_BODY(WindowSystem);

	public:
		WindowSystem(engine::Engine& engine);

		~WindowSystem() override;

	public:
		M_FIELD_BY_REF_3(public, engine::Engine, Engine);

	// ----------------------------------------------------
	// Window

	public:
		M_FIELD_BY_VAR_4(public, private, std::shared_ptr<IWindow>, Window);

	public:
		M_FIELD_FOR_EVENT_2(public, OnWindowInit);

		M_FIELD_FOR_EVENT_2(public, OnWindowTick);

		M_FIELD_FOR_EVENT_2(public, OnWindowShut);

	public:
		void Run();

	private:
		void InitWindow();

		void ShutWindow();

	private:
		void Window_When_Window_OnInit();

		void Window_When_Window_OnTick();

		void WIndow_When_Window_OnShut();
	};
}