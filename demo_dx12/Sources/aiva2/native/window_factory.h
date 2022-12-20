#pragma once
#include <pch.h>

#include <aiva2/engine/i_window_fwd.h>

namespace aiva2::native
{
	struct WindowFactory final
	{
	// ----------------------------------------------------

	private:
		WindowFactory() = delete;

	// ----------------------------------------------------

	public:
		static auto Create() -> std::shared_ptr<engine::IWindow>;
	};
}
