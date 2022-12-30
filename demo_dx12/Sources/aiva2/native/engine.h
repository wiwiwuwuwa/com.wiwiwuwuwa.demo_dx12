#pragma once
#include <pch.h>

#include <aiva2/engine/i_object.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/native/engine_fwd.h>
#include <aiva2/native/graphic_hardware_fwd.h>
#include <aiva2/native/window_system_fwd.h>

namespace aiva2::native
{
	struct Engine final : public engine::IObject
	{
	// ----------------------------------------------------
	// Main

	public:
		M_STRUCT_BODY(Engine);

	public:
		Engine();

		~Engine() override;

	public:
		void Run() const;

	// ----------------------------------------------------
	// Systems

	private:
		void InitSystems();

		void ShutSystems();

	private:
		void Systems_When_WindowSystem_OnWindowInit();

		void Systems_When_WindowSystem_OnWindowShut();

	// ----------------------------------------------------

	public:
		auto WindowSystem() const -> native::WindowSystem&;

	private:
		void InitWindowSystem();

		void ShutWindowSystem();

	private:
		std::shared_ptr<native::WindowSystem> mWindowSystem{};

	// ----------------------------------------------------

	public:
		auto GraphicHardware() const -> native::GraphicHardware&;

	private:
		void InitGraphicHardware();

		void ShutGraphicHardware();

	private:
		std::shared_ptr<native::GraphicHardware> mGraphicHardware{};
	};
}
