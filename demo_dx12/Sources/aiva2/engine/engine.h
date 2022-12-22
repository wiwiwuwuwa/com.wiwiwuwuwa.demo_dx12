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
	struct Engine : public IObject
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
	// IObject

	public:
		void Init() override;

		void Shut() override;

	// ----------------------------------------------------
	// Systems

	private:
		void InitSystems();

		void ShutSystems();

	// ----------------------------------------------------
	// Systems (Generic)

	public:
		auto TimeSystem() const -> engine::TimeSystem&;

	private:
		void InitTimeSystem();

		void ShutTimeSystem();

	private:
		std::shared_ptr<engine::TimeSystem> mTimeSystem{};

	// ----------------------------------------------------
	// Systems (Virtual)

	public:
		virtual auto WindowSystem() const -> engine::WindowSystem& = NULL;

	protected:
		virtual void InitWindowSystem() = NULL;

		virtual void ShutWindowSystem() = NULL;

	// ----------------------------------------------------

	public:
		virtual auto GraphicHardware() const -> engine::GraphicHardware& = NULL;

	protected:
		virtual void InitGraphicHardware() = NULL;

		virtual void ShutGraphicHardware() = NULL;
	};
}
