#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/core/object.hpp>

namespace aiva2::native
{
	struct engine_t final : public core::implements_t<engine_t, core::object_t>
	{
		// ------------------------------------------------
		// engine

	public:
		engine_t();

		~engine_t() override;

		// ------------------------------------------------

	public:
		void run() const;

		// ------------------------------------------------
		// systems

	public:
		auto get_window_system() const->window_system_t&;

		auto get_time_system() const->time_system_t&;

		auto get_graphic_hardware() const->graphic_hardware_t&;

	private:
		void init_systems();

		void shut_systems();

		void init_window_system();

		void shut_window_system();

		void init_time_system();

		void tick_time_system();

		void shut_time_system();

		void init_graphic_hardware();

		void shut_graphic_hardware();

		void systems_when_window_system_on_init();

		void systems_when_window_system_on_tick();

		void systems_when_window_system_on_shut();

	private:
		std::unique_ptr<window_system_t> m_window_system{};

		std::unique_ptr<time_system_t> m_time_system{};

		std::unique_ptr<graphic_hardware_t> m_graphic_hardware{};
	};
}
