#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct render_texture_2d_t final : public implements_t<render_texture_2d_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		render_texture_2d_t(engine_t& engine);

		~render_texture_2d_t() override;
		
	public:
		auto get_color_cpu_descriptor_handles() const -> std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>;

		auto get_depth_cpu_descriptor_handle() const -> std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;

		void set_state_for_transition() const;

		void set_state_for_uav() const;

		// ------------------------------------------------
		
	public:
		void add_color_buffer(std::shared_ptr<render_color_buffer_2d_t> const& buffer);
		
		template <typename... t_buffer_args>
		void add_color_buffer(t_buffer_args&&... args);
		
		auto get_color_buffer(size_t const index) const->std::shared_ptr<render_color_buffer_2d_t> const&;

		void rem_color_buffer(size_t const index);
		
		auto num_color_buffers() const->size_t;
		
	private:
		std::vector<std::shared_ptr<render_color_buffer_2d_t>> m_color_buffers{};
		
		// ------------------------------------------------
		
	public:
		auto get_depth_buffer() const->std::shared_ptr<render_depth_buffer_2d_t> const&;

		auto has_depth_buffer() const->bool;
		
		void set_depth_buffer(std::shared_ptr<render_depth_buffer_2d_t> const& buffer);
		
		template <typename... t_buffer_args>
		void set_depth_buffer(t_buffer_args&&... args);
		
	private:
		std::shared_ptr<render_depth_buffer_2d_t> m_depth_buffer{};

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2
{
	template <typename... t_buffer_args>
	void render_texture_2d_t::add_color_buffer(t_buffer_args&&... args)
	{
		add_color_buffer(std::make_shared<render_color_buffer_2d_t>(std::forward<t_buffer_args>(args)...));
	}

	template <typename... t_buffer_args>
	void render_texture_2d_t::set_depth_buffer(t_buffer_args&&... args)
	{
		set_depth_buffer(std::make_shared<render_depth_buffer_2d_t>(std::forward<t_buffer_args>(args)...));
	}
}
