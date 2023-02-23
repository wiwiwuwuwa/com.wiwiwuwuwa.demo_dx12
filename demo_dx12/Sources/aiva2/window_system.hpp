#pragma once
#include <aiva2/_core.hpp>
#include <aiva2/implements.hpp>
#include <aiva2/engine_object.hpp>

namespace aiva2
{
	struct window_system_t final : public implements_t<window_system_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		window_system_t(engine_t& engine);

		~window_system_t() override;

	public:
		void run() const;

		// ------------------------------------------------

	public:
		auto get_base_window() const->window_t&;

		auto get_core_window() const->CoreWindow const&;

		auto get_on_window_init() -> event_action_type<>&;

		auto get_on_window_tick() -> event_action_type<>&;

		auto get_on_window_shut() -> event_action_type<>&;

	private:
		void init_window();

		void shut_window();

	private:
		void window_system_when_window_on_init();

		void window_system_when_window_on_tick();

		void window_system_when_window_on_shut();

	private:
		winrt::com_ptr<window_t> m_base_window{};

		event_action_type<> m_on_window_init{};
		
		event_action_type<> m_on_window_tick{};

		event_action_type<> m_on_window_shut{};

		// ------------------------------------------------
	};
}
