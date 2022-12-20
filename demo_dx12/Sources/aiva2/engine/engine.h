#pragma once
#include <pch.h>

#include <aiva2/engine/engine_fwd.h>
#include <aiva2/engine/i_object.h>
#include <aiva2/engine/i_window_fwd.h>
#include <aiva2/engine/m_field_by_var.h>
#include <aiva2/engine/m_field_for_event.h>
#include <aiva2/engine/m_struct_body.h>

namespace aiva2::engine
{
	struct Engine final : public IObject
	{
	// ----------------------------------------------------
	// Main

	public:
		M_STRUCT_BODY(Engine);

	public:
		Engine();

		~Engine() override;

	// ----------------------------------------------------

	public:
		M_FIELD_FOR_EVENT_2(public, OnInit);

		M_FIELD_FOR_EVENT_2(public, OnTick);

		M_FIELD_FOR_EVENT_2(public, OnDraw);

		M_FIELD_FOR_EVENT_2(public, OnShut);

	public:
		void Run();

	// ----------------------------------------------------
	// Window

	public:
		M_FIELD_BY_VAR_3(private, std::shared_ptr<IWindow>, Window);

	private:
		void InitWindow();

		void ShutWindow();

	private:
		void Window_OnInit();

		void Window_OnTick();

		void Window_OnShut();
	};
}