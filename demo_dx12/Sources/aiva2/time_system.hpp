#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct time_system_t final : public implements_t<time_system_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		time_system_t(engine_t& engine);
		
		~time_system_t() override;
		
	public:
		auto get_on_update() -> event_action_type<>&;
		
		auto get_on_render() -> event_action_type<>&;

		auto get_tick() const->size_t;
		
		void tick();
		
	private:
		void init_time_system();

		void tick_time_system();

		void shut_time_system();

	private:
		event_action_type<> m_on_update{};

		event_action_type<> m_on_render{};

		size_t m_tick{};
		
		// ------------------------------------------------
	};
}
