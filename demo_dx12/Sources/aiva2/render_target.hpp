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
		template <typename t_gpu_res>
		using rtv_eye_info_type = find_gpu_eye_info_b_t<gpu_eye_type_t::RENDER_TARGET, t_gpu_res>;

	private:
		template <typename t_gpu_res>
		using rtv_eye_type = find_gpu_eye_b_t<gpu_eye_type_t::RENDER_TARGET, t_gpu_res>;

	public:
		template <typename t_gpu_res>
		void add_rtv_res(std::shared_ptr<t_gpu_res> const& rtv_res, rtv_eye_info_type<t_gpu_res> const& rtv_info = {});

		template <typename t_gpu_res>
		void set_rtv_res(size_t const index, std::shared_ptr<t_gpu_res> const& rtv_res, rtv_eye_info_type<t_gpu_res> const& rtv_info = {});

	public:
		auto num_rtv_res() const->size_t;

		void rem_rtv_res(size_t const index);

		// ------------------------------------------------
		// PUBLIC DEPTH STENCIL API
		// ------------------------------------------------

	public:
		template <typename t_gpu_res>
		using dsv_eye_info_type = find_gpu_eye_info_b_t<gpu_eye_type_t::DEPTH_STENCIL, t_gpu_res>;

	private:
		template <typename t_gpu_res>
		using dsv_eye_type = find_gpu_eye_b_t<gpu_eye_type_t::DEPTH_STENCIL, t_gpu_res>;

	public:
		template <typename t_gpu_res>
		void set_dsv_res(std::shared_ptr<t_gpu_res> const& dsv_res, dsv_eye_info_type<t_gpu_res> const& dsv_info = {});

	public:
		auto has_dsv_res() const->bool;

		// ------------------------------------------------
		// PRIVATE RENDER TARGET API
		// ------------------------------------------------

	private:
		void add_rtv_eye(std::shared_ptr<rtv_eye_t> const& rtv_eye);
		
		auto get_rtv_eye(size_t const index) const->std::shared_ptr<rtv_eye_t> const&;
		
		auto num_rtv_eye() const->size_t;
		
		void rem_rtv_eye(size_t const index);
		
		void set_rtv_eye(size_t const index, std::shared_ptr<rtv_eye_t> const& rtv_eye);

	private:
		auto init_rtv_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		auto shut_rtv_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		std::vector<std::shared_ptr<rtv_eye_t>> m_rtv_eyes{};

		// ------------------------------------------------
		// PRIVATE DEPTH STENCIL API
		// ------------------------------------------------

	private:
		auto get_dsv_eye() const->std::shared_ptr<dsv_eye_t> const&;

		auto has_dsv_eye() const->bool;

		void set_dsv_eye(std::shared_ptr<dsv_eye_t> const& dsv_eye);

	private:
		auto init_dsv_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		auto shut_dsv_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		std::shared_ptr<dsv_eye_t> m_dsv_eye{};

		// ------------------------------------------------
		// PUBLIC RENDER HANDLE API
		// ------------------------------------------------

	public:
		auto get_rtv_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;

		auto num_rtv_handle() const->size_t;

	private:
		void upd_rtv_handle();
		
	private:
		winrt::com_ptr<ID3D12DescriptorHeap> m_rtv_heap{};

		// ------------------------------------------------
		// PUBLIC DEPTH HANDLE API
		// ------------------------------------------------

	public:
		auto get_dsv_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;

	private:
		void upd_dsv_handle();
		
	private:
		winrt::com_ptr<ID3D12DescriptorHeap> m_dsv_heap{};
		
		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2
{
	template <typename t_gpu_res>
	void render_target_t::add_rtv_res(std::shared_ptr<t_gpu_res> const& rtv_res, rtv_eye_info_type<t_gpu_res> const& rtv_info)
	{
		using rtv_eye_t = rtv_eye_type<t_gpu_res>;

		auto const rtv_eye = std::make_shared<rtv_eye_t>(get_engine(), rtv_res, rtv_info);
		assert_t::check_bool(rtv_eye, "rtv_eye is not valid");

		add_rtv_eye(rtv_eye);
	}

	template <typename t_gpu_res>
	void render_target_t::set_rtv_res(size_t const index, std::shared_ptr<t_gpu_res> const& rtv_res, rtv_eye_info_type<t_gpu_res> const& rtv_info)
	{
		using rtv_eye_t = rtv_eye_type<t_gpu_res>;

		auto const rtv_eye = std::make_shared<rtv_eye_t>(get_engine(), rtv_res, rtv_info);
		assert_t::check_bool(rtv_eye, "rtv_eye is not valid");

		set_rtv_eye(index, rtv_eye);
	}

	template <typename t_gpu_res>
	void render_target_t::set_dsv_res(std::shared_ptr<t_gpu_res> const& dsv_res, dsv_eye_info_type<t_gpu_res> const& dsv_info)
	{
		using dsv_eye_t = dsv_eye_type<t_gpu_res>;

		auto const dsv_eye = std::make_shared<dsv_eye_t>(get_engine(), dsv_res, dsv_info);
		assert_t::check_bool(dsv_eye, "dsv_eye is not valid");

		set_dsv_eye(dsv_eye);
	}
}
