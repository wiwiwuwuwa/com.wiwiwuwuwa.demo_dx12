#pragma once
#include <pch.h>

#include <aiva2/engine/engine_fwd.h>
#include <aiva2/engine/i_object.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/engine/t_event_action.h>
#include <aiva2/engine/window_system_fwd.h>

namespace aiva2::engine
{
	struct WindowSystem : public IObject
	{
	// ----------------------------------------------------
	// Main

	public:
		M_STRUCT_BODY(WindowSystem);

	public:
		WindowSystem();

		~WindowSystem() override;

	// ----------------------------------------------------
	// Engine (Virtual)

	public:
		virtual auto Engine() const -> engine::Engine& = NULL;

	// ----------------------------------------------------
	// Window (Virtual)

	public:
		virtual auto OnWindowInit() -> EventActionReadOnly& = NULL;

		virtual auto OnWindowTick() -> EventActionReadOnly& = NULL;

		virtual auto OnWindowShut() -> EventActionReadOnly& = NULL;

	public:
		virtual void RunWindow() = NULL;
	};
}
