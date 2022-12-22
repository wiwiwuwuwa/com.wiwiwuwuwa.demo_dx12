#pragma once
#include <pch.h>

#include <aiva2/engine/engine.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/native/uwp/engine_fwd.h>
#include <aiva2/native/uwp/graphic_hardware.h>
#include <aiva2/native/uwp/window_system.h>

namespace aiva2::native
{
	struct Engine final : public engine::Engine
	{
	// ----------------------------------------------------
	// Main

	public:
		M_STRUCT_BODY(Engine);

	public:
		Engine();

		~Engine() override;

	// ----------------------------------------------------
	// Systems (Virtual)

	public:
		native::WindowSystem& WindowSystem() const override;

	protected:
		void InitWindowSystem() override;

		void ShutWindowSystem() override;

	private:
		std::shared_ptr<native::WindowSystem> mWindowSystem{};

	// ----------------------------------------------------

	public:
		native::GraphicHardware& GraphicHardware() const override;

	protected:
		void InitGraphicHardware() override;

		void ShutGraphicHardware() override;

	private:
		std::shared_ptr<native::GraphicHardware> mGraphicHardware{};
	};
}
