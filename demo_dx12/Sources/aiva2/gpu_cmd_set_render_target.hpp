#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_cmd_base.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct gpu_cmd_set_render_target_t final : public implements_t<gpu_cmd_set_render_target_t, gpu_cmd_base_t>
	{
		// ------------------------------------------------

	public:
		gpu_cmd_set_render_target_t(engine_t& engine);

		~gpu_cmd_set_render_target_t() override;

	public:
		void execute() override;

	private:
		void execute_resource_barrier() const;
		
		void execute_set_render_targets() const;

		// ------------------------------------------------

	public:
		auto get_render_target_ptr() const->std::shared_ptr<render_target_t> const&;

		auto get_render_target_ref() const->render_target_t&;

		void set_render_target_ptr(std::shared_ptr<render_target_t> const& render_target);
		
	private:
		std::shared_ptr<render_target_t> m_render_target{};

		// ------------------------------------------------
	};
}