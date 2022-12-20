#pragma once
#include <pch.h>

#include <aiva2/engine/i_window_fwd.h>
#include <aiva2/engine/t_event_action.h>

namespace aiva2::engine
{
	struct IWindow
	{
	// ----------------------------------------------------

	public:
		virtual ~IWindow() = default;

	// ----------------------------------------------------

	public:
		virtual auto OnInit() -> EventActionReadOnly& = NULL;

		virtual auto OnTick() -> EventActionReadOnly& = NULL;

		virtual auto OnShut() -> EventActionReadOnly& = NULL;

	public:
		virtual void Run() = NULL;
	};
}
