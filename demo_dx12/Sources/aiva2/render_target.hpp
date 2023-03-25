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
		void add_rtv_eye(std::shared_ptr<rtv_eye_t> const& rtv_eye);
		
		auto get_rtv_eye(size_t const index) const->std::shared_ptr<rtv_eye_t> const&;
		
		auto num_rtv_eye() const->size_t;
		
		void rem_rtv_eye(size_t const index);
		
		void set_rtv_eye(size_t const index, std::shared_ptr<rtv_eye_t> const& rtv_eye);

	public:
		template <typename t_rtv_eye_type, typename... t_rtv_eye_args>
		void add_rtv_eye(t_rtv_eye_args&&... render_texture_args);

		template <typename t_rtv_eye_type, typename... t_rtv_eye_args>
		void set_rtv_eye(size_t const index, t_rtv_eye_args&&... render_texture_args);

	private:
		auto init_rtv_eye_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		auto shut_rtv_eye_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		std::vector<std::shared_ptr<rtv_eye_t>> m_rtv_eyes{};

		// ------------------------------------------------

	public:
		auto get_rtv_eye_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;

		auto num_rtv_eye_handle() const->size_t;

	private:
		void upd_rtv_eye_handle();
		
	private:
		winrt::com_ptr<ID3D12DescriptorHeap> m_rtv_eye_heap{};

		// ------------------------------------------------

	public:
		auto get_dsv_eye() const->std::shared_ptr<dsv_eye_t> const&;

		auto has_dsv_eye() const->bool;

		void set_dsv_eye(std::shared_ptr<dsv_eye_t> const& dsv_eye);
		
	public:
		template <typename t_dsv_eye_type, typename... t_dsv_eye_args>
		void set_dsv_eye(t_dsv_eye_args&&... render_texture_args);

	private:
		auto init_dsv_eye_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

		auto shut_dsv_eye_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		std::shared_ptr<dsv_eye_t> m_dsv_eye{};

		// ------------------------------------------------

	public:
		auto get_dsv_eye_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;

	private:
		void upd_dsv_eye_handle();
		
	private:
		winrt::com_ptr<ID3D12DescriptorHeap> m_dsv_eye_heap{};

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2
{
	template <typename t_rtv_eye_type, typename... t_rtv_eye_args>
	void render_target_t::add_rtv_eye(t_rtv_eye_args&&... render_texture_args)
	{
		auto const rtv_eye = std::make_shared<t_rtv_eye_type>(get_engine(), std::forward<t_rtv_eye_args>(render_texture_args)...);
		assert_t::check_bool(rtv_eye, "rtv_eye is not valid");
		
		add_rtv_eye(rtv_eye);
	}

	template <typename t_rtv_eye_type, typename... t_rtv_eye_args>
	void render_target_t::set_rtv_eye(size_t const index, t_rtv_eye_args&&... render_texture_args)
	{
		auto const rtv_eye = std::make_shared<t_rtv_eye_type>(get_engine(), std::forward<t_rtv_eye_args>(render_texture_args)...);
		assert_t::check_bool(rtv_eye, "rtv_eye is not valid");

		set_rtv_eye(index, rtv_eye);
	}

	template <typename t_dsv_eye_type, typename... t_dsv_eye_args>
	void render_target_t::set_dsv_eye(t_dsv_eye_args&&... render_texture_args)
	{
		auto const dsv_eye = std::make_shared<t_dsv_eye_type>(get_engine(), std::forward<t_dsv_eye_args>(render_texture_args)...);
		assert_t::check_bool(dsv_eye, "dsv_eye is not valid");

		set_dsv_eye(dsv_eye);
	}
}
