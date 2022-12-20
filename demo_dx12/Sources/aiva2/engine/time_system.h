#pragma once
#include <pch.h>

#include <aiva2/engine/engine_fwd.h>
#include <aiva2/engine/i_object.h>
#include <aiva2/engine/m_field_by_ref.h>
#include <aiva2/engine/m_field_for_event.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/engine/time_system_fwd.h>

namespace aiva2::engine
{
	struct TimeSystem final : public IObject
	{
	// ----------------------------------------------------
	// Main

	public:
		M_STRUCT_BODY(TimeSystem);

	public:
		TimeSystem(engine::Engine& engine);

		~TimeSystem() override;

	public:
		M_FIELD_BY_REF_3(public, engine::Engine, Engine);

	// ----------------------------------------------------
	// Time

	public:
		M_FIELD_FOR_EVENT_2(public, OnTimeInit);

		M_FIELD_FOR_EVENT_2(public, OnTimeTick);

		M_FIELD_FOR_EVENT_2(public, OnTimeDraw);

		M_FIELD_FOR_EVENT_2(public, OnTimeShut);

	private:
		void InitTime();

		void ShutTime();

	private:
		void Time_When_WindowSystem_OnWindowInit();

		void Time_When_WindowSystem_OnWindowTick();

		void Time_When_WindowSystem_OnWindowShut();
	};
}
