#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct render_target_t final : public implements_t<render_target_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		render_target_t(engine_t& engine);
		
		~render_target_t() override;

	public:
		void set_state_for_transition() const;
		
		void set_state_for_uav() const;

		// ------------------------------------------------

	public:
		void add_color_target(std::shared_ptr<render_color_target_t> const& color_target);
		
		auto get_color_target(size_t const index) const->std::shared_ptr<render_color_target_t> const&;
		
		auto num_color_target() const->size_t;
		
		void rem_color_target(size_t const index);
		
		void set_color_target(size_t const index, std::shared_ptr<render_color_target_t> const& color_target);

	public:
		auto get_color_cpu_descriptor_handles() const->std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>;

	private:
		std::vector<std::shared_ptr<render_color_target_t>> m_color_targets{};

		// ------------------------------------------------

	public:
		auto get_depth_target() const->std::shared_ptr<render_depth_target_t> const&;

		auto has_depth_target() const->bool;

		void set_depth_target(std::shared_ptr<render_depth_target_t> const& depth_target);

	public:
		auto get_depth_cpu_descriptor_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;

	private:
		std::shared_ptr<render_depth_target_t> m_depth_target{};

		// ------------------------------------------------
	};
}
