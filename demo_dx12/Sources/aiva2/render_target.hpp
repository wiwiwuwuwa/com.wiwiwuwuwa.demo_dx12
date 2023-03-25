#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/find_gpu_eye_b.hpp>
#include <aiva2/find_gpu_eye_info_b.hpp>
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
		// PUBLIC RENDER TARGET API
		// ------------------------------------------------

	public:
		template <typename t_resource>
		using color_info_type = find_gpu_eye_info_b_t<gpu_eye_type_t::RENDER_TARGET, t_resource>;

	private:
		template <typename t_resource>
		using color_view_type = find_gpu_eye_b_t<gpu_eye_type_t::RENDER_TARGET, t_resource>;

	public:
		template <typename t_resource>
		void add_color_resource(std::shared_ptr<t_resource> const& color_resource, color_info_type<t_resource> const& color_info = {});

		template <typename t_resource>
		void set_color_resource(size_t const index, std::shared_ptr<t_resource> const& color_resource, color_info_type<t_resource> const& color_info = {});

	public:
		auto num_color_resource() const->size_t;

		void rem_color_resource(size_t const index);

		// ------------------------------------------------
		// PUBLIC DEPTH STENCIL API
		// ------------------------------------------------

	public:
		template <typename t_resource>
		using depth_info_type = find_gpu_eye_info_b_t<gpu_eye_type_t::DEPTH_STENCIL, t_resource>;

	private:
		template <typename t_resource>
		using depth_view_type = find_gpu_eye_b_t<gpu_eye_type_t::DEPTH_STENCIL, t_resource>;

	public:
		template <typename t_resource>
		void set_depth_resource(std::shared_ptr<t_resource> const& depth_resource, depth_info_type<t_resource> const& depth_info = {});

	public:
		auto has_depth_resource() const->bool;

		// ------------------------------------------------
		// PRIVATE RENDER TARGET API
		// ------------------------------------------------

	private:
		void add_color_view(std::shared_ptr<rtv_eye_t> const& color_view);
		
		auto get_color_view(size_t const index) const->std::shared_ptr<rtv_eye_t> const&;
		
		auto num_color_view() const->size_t;
		
		void rem_color_view(size_t const index);
		
		void set_color_view(size_t const index, std::shared_ptr<rtv_eye_t> const& color_view);

	private:
		auto init_color_views_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		auto shut_color_views_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		std::vector<std::shared_ptr<rtv_eye_t>> m_color_views{};

		// ------------------------------------------------
		// PRIVATE DEPTH STENCIL API
		// ------------------------------------------------

	private:
		auto get_depth_view() const->std::shared_ptr<dsv_eye_t> const&;

		auto has_depth_view() const->bool;

		void set_depth_view(std::shared_ptr<dsv_eye_t> const& depth_view);

	private:
		auto init_depth_view_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		auto shut_depth_view_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		std::shared_ptr<dsv_eye_t> m_depth_view{};

		// ------------------------------------------------
		// PUBLIC RENDER HANDLE API
		// ------------------------------------------------

	public:
		auto get_color_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;

		auto num_color_handle() const->size_t;

	private:
		void upd_color_handle();
		
	private:
		winrt::com_ptr<ID3D12DescriptorHeap> m_color_heap{};

		// ------------------------------------------------
		// PUBLIC DEPTH HANDLE API
		// ------------------------------------------------

	public:
		auto get_depth_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;

	private:
		void upd_depth_handle();
		
	private:
		winrt::com_ptr<ID3D12DescriptorHeap> m_depth_heap{};
		
		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2
{
	template <typename t_resource>
	void render_target_t::add_color_resource(std::shared_ptr<t_resource> const& color_resource, color_info_type<t_resource> const& color_info)
	{
		using color_view_t = color_view_type<t_resource>;

		auto const color_view = std::make_shared<color_view_t>(get_engine(), color_resource, color_info);
		assert_t::check_bool(color_view, "color_view is not valid");

		add_color_view(color_view);
	}

	template <typename t_resource>
	void render_target_t::set_color_resource(size_t const index, std::shared_ptr<t_resource> const& color_resource, color_info_type<t_resource> const& color_info)
	{
		using color_view_t = color_view_type<t_resource>;

		auto const color_view = std::make_shared<color_view_t>(get_engine(), color_resource, color_info);
		assert_t::check_bool(color_view, "color_view is not valid");

		set_color_view(index, color_view);
	}

	template <typename t_resource>
	void render_target_t::set_depth_resource(std::shared_ptr<t_resource> const& depth_resource, depth_info_type<t_resource> const& depth_info)
	{
		using depth_view_t = depth_view_type<t_resource>;

		auto const depth_view = std::make_shared<depth_view_t>(get_engine(), depth_resource, depth_info);
		assert_t::check_bool(depth_view, "depth_view is not valid");

		set_depth_view(depth_view);
	}
}
