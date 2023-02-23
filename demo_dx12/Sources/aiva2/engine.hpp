#pragma once
#include <aiva2/_core.hpp>
#include <aiva2/implements.hpp>
#include <aiva2/object.hpp>

namespace aiva2
{
	struct engine_t final : public implements_t<engine_t, object_t>
	{
		// ------------------------------------------------

	public:
		engine_t();

		~engine_t() override;

	public:
		void run() const;

		// ------------------------------------------------

	public:
		auto get_window_system() const->window_system_t&;

	private:
		void init_window_system();

		void shut_window_system();

	private:
		void engine_when_window_system_on_window_init();

		void engine_when_window_system_on_window_tick();

		void engine_when_window_system_on_window_shut();

	private:
		std::unique_ptr<window_system_t> m_window_system{};

		// ------------------------------------------------
	};
}
