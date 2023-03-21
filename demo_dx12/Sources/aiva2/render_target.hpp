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
		auto init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		auto shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		// ------------------------------------------------

	public:
		void add_color_texture(std::shared_ptr<rtv_eye_t> const& color_texture);
		
		auto get_color_texture(size_t const index) const->std::shared_ptr<rtv_eye_t> const&;
		
		auto num_color_texture() const->size_t;
		
		void rem_color_texture(size_t const index);
		
		void set_color_texture(size_t const index, std::shared_ptr<rtv_eye_t> const& color_texture);

	public:
		template <typename t_color_texture_type, typename... t_color_texture_args>
		void add_color_texture(t_color_texture_args&&... render_texture_args);

		template <typename t_color_texture_type, typename... t_color_texture_args>
		void set_color_texture(size_t const index, t_color_texture_args&&... render_texture_args);

	private:
		auto init_color_texture_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		auto shut_color_texture_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		std::vector<std::shared_ptr<rtv_eye_t>> m_color_textures{};

		// ------------------------------------------------

	public:
		auto get_color_texture_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;

		auto num_color_texture_handle() const->size_t;

	private:
		void upd_color_texture_handle();
		
	private:
		winrt::com_ptr<ID3D12DescriptorHeap> m_color_heap{};

		// ------------------------------------------------

	public:
		auto get_depth_texture() const->std::shared_ptr<dsv_eye_t> const&;

		auto has_depth_texture() const->bool;

		void set_depth_texture(std::shared_ptr<dsv_eye_t> const& depth_texture);
		
	public:
		template <typename t_depth_texture_type, typename... t_depth_texture_args>
		void set_depth_texture(t_depth_texture_args&&... render_texture_args);

	private:
		auto init_depth_texture_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		auto shut_depth_texture_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		std::shared_ptr<dsv_eye_t> m_depth_texture{};

		// ------------------------------------------------

	public:
		auto get_depth_texture_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;

	private:
		void upd_depth_texture_handle();
		
	private:
		winrt::com_ptr<ID3D12DescriptorHeap> m_depth_heap{};

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2
{
	template <typename t_color_texture_type, typename... t_color_texture_args>
	void render_target_t::add_color_texture(t_color_texture_args&&... render_texture_args)
	{
		auto const color_texture = std::make_shared<t_color_texture_type>(get_engine(), std::forward<t_color_texture_args>(render_texture_args)...);
		assert_t::check_bool(color_texture, "color_texture is not valid");
		
		add_color_texture(color_texture);
	}

	template <typename t_color_texture_type, typename... t_color_texture_args>
	void render_target_t::set_color_texture(size_t const index, t_color_texture_args&&... render_texture_args)
	{
		auto const color_texture = std::make_shared<t_color_texture_type>(get_engine(), std::forward<t_color_texture_args>(render_texture_args)...);
		assert_t::check_bool(color_texture, "color_texture is not valid");

		set_color_texture(index, color_texture);
	}

	template <typename t_depth_texture_type, typename... t_depth_texture_args>
	void render_target_t::set_depth_texture(t_depth_texture_args&&... render_texture_args)
	{
		auto const depth_texture = std::make_shared<t_depth_texture_type>(get_engine(), std::forward<t_depth_texture_args>(render_texture_args)...);
		assert_t::check_bool(depth_texture, "depth_texture is not valid");

		set_depth_texture(depth_texture);
	}
}
