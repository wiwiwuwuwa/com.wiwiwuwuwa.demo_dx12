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

	public:
		auto get_resource_system() const->resource_system_t&;

	private:
		void init_resource_system();

		void shut_resource_system();

	private:
		std::unique_ptr<resource_system_t> m_resource_system{};

		// ------------------------------------------------

	public:
		auto get_time_system() const->time_system_t&;

	private:
		void init_time_system();

		void tick_time_system();

		void shut_time_system();
		
	private:
		std::unique_ptr<time_system_t> m_time_system{};

		// ------------------------------------------------

	public:
		auto get_graphic_hardware() const->graphic_hardware_t&;
		
	private:
		void init_graphic_hardware();

		void shut_graphic_hardware();

	private:
		std::unique_ptr<graphic_hardware_t> m_graphic_hardware{};

		// ------------------------------------------------

	public:
		auto get_graphic_pipeline() const->graphic_pipeline_t&;
		
	private:
		void init_graphic_pipeline();
		
		void shut_graphic_pipeline();

	private:
		std::unique_ptr<graphic_pipeline_t> m_graphic_pipeline{};

		// ------------------------------------------------

	public:
		auto get_graphic_executor() const->graphic_executor_t&;
		
	private:
		void init_graphic_executor();
		
		void shut_graphic_executor();
		
	private:
		std::unique_ptr<graphic_executor_t> m_graphic_executor{};

		// ------------------------------------------------

	public:
		auto get_graphic_renderer() const->graphic_renderer_t&;

	private:
		void init_graphic_renderer();

		void shut_graphic_renderer();
		
	private:
		std::unique_ptr<graphic_renderer_t> m_graphic_renderer{};

		// ------------------------------------------------

	public:
		auto get_deffered_renderer() const->deffered_renderer_t&;

	private:
		void init_deffered_renderer();

		void shut_deffered_renderer();

	private:
		std::unique_ptr<deffered_renderer_t> m_deffered_renderer{};

		// ------------------------------------------------
	};
}
