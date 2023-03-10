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
		void add_color_texture(std::shared_ptr<render_texture_base_t> const& color_texture);
		
		auto get_color_texture(size_t const index) const->std::shared_ptr<render_texture_base_t> const&;
		
		auto num_color_texture() const->size_t;
		
		void rem_color_texture(size_t const index);
		
		void set_color_texture(size_t const index, std::shared_ptr<render_texture_base_t> const& color_texture);

	public:
		auto get_color_cpu_descriptor_handles() const->std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>;

	public:
		template <typename t_render_texture_struct, typename... t_render_texture_args>
		void add_color_texture(t_render_texture_args&&... render_texture_args);

		template <typename t_render_texture_struct, typename... t_render_texture_args>
		void set_color_texture(size_t const index, t_render_texture_args&&... render_texture_args);

	private:
		std::vector<std::shared_ptr<render_texture_base_t>> m_color_textures{};

		// ------------------------------------------------

	public:
		auto get_depth_texture() const->std::shared_ptr<render_texture_base_t> const&;

		auto has_depth_texture() const->bool;

		void set_depth_texture(std::shared_ptr<render_texture_base_t> const& depth_texture);

	public:
		auto get_depth_cpu_descriptor_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;
		
	public:
		template <typename t_render_texture_struct, typename... t_render_texture_args>
		void set_depth_texture(t_render_texture_args&&... render_texture_args);

	private:
		std::shared_ptr<render_texture_base_t> m_depth_texture{};

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2
{
	template <typename t_render_texture_struct, typename... t_render_texture_args>
	void render_target_t::add_color_texture(t_render_texture_args&&... render_texture_args)
	{
		auto const color_texture = std::make_shared<t_render_texture_struct>(get_engine(), std::forward<t_render_texture_args>(render_texture_args)...);
		assert_t::check_bool(color_texture, "color_texture is not valid");
		
		add_color_texture(color_texture);
	}

	template <typename t_render_texture_struct, typename... t_render_texture_args>
	void render_target_t::set_color_texture(size_t const index, t_render_texture_args&&... render_texture_args)
	{
		auto const color_texture = std::make_shared<t_render_texture_struct>(get_engine(), std::forward<t_render_texture_args>(render_texture_args)...);
		assert_t::check_bool(color_texture, "color_texture is not valid");

		set_color_texture(index, color_texture);
	}

	template <typename t_render_texture_struct, typename... t_render_texture_args>
	void render_target_t::set_depth_texture(t_render_texture_args&&... render_texture_args)
	{
		auto const depth_texture = std::make_shared<t_render_texture_struct>(get_engine(), std::forward<t_render_texture_args>(render_texture_args)...);
		assert_t::check_bool(depth_texture, "depth_texture is not valid");

		set_depth_texture(depth_texture);
	}
}
