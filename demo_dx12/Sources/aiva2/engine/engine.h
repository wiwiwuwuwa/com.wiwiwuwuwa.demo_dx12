#pragma once
#include <pch.h>

#include <aiva2/engine/engine_fwd.h>
#include <aiva2/engine/graphic_hardware_fwd.h>
#include <aiva2/engine/i_object.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/engine/time_system_fwd.h>
#include <aiva2/engine/window_system_fwd.h>

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

	public:
		void Run();

	// ----------------------------------------------------
	// Systems

	private:
		void InitSystems();

		void ShutSystems();

	// ----------------------------------------------------

	public:
		engine::WindowSystem& WindowSystem();

	private:
		void InitWindowSystem();

		void ShutWindowSystem();

	private:
		std::shared_ptr<engine::WindowSystem> mWindowSystem{};

	// ----------------------------------------------------

	public:
		engine::TimeSystem& TimeSystem();

	private:
		void InitTimeSystem();

		void ShutTimeSystem();

	private:
		std::shared_ptr<engine::TimeSystem> mTimeSystem{};

	// ----------------------------------------------------

	public:
		engine::GraphicHardware& GraphicHardware();

	private:
		void InitGraphicHardware();

		void ShutGraphicHardware();

	private:
		std::shared_ptr<engine::GraphicHardware> mGraphicHardware{};
	};
}
