#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/native/engine_object.hpp>

namespace aiva2::native
{
	struct window_system_t final : public core::implements_t<window_system_t, engine_object_t>
	{
		// ------------------------------------------------
		// window system

	public:
		window_system_t(engine_t& engine);

		~window_system_t() override;

		// ------------------------------------------------

	public:
		auto get_on_init() -> event_action_type<>&;

		auto get_on_tick() -> event_action_type<>&;

		auto get_on_shut() -> event_action_type<>&;

		void run() const;

	private:
		void init_window_system();

		void shut_window_system();

		void window_system_when_window_on_init();

		void window_system_when_window_on_tick();

		void window_system_when_window_on_shut();

	private:
		event_action_type<> m_on_init{};

		event_action_type<> m_on_tick{};

		event_action_type<> m_on_shut{};

		// ------------------------------------------------
		// window

	public:
		auto get_base_window() const->window_t&;

		auto get_core_window() const->CoreWindow const&;

	private:
		void init_window();

		void shut_window();

	private:
		winrt::com_ptr<window_t> m_base_window{};

		// ------------------------------------------------
	};
}
