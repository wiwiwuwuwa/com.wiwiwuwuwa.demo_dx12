#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/rtv_eye.hpp>
#include <aiva2/rtv_tex_2d_info.hpp>
#include <aiva2/tex_2d_info.hpp>

namespace aiva2
{
	struct rtv_tex_2d_t final : public implements_t<rtv_tex_2d_t, rtv_eye_t>
	{
		// ------------------------------------------------

	public:
		rtv_tex_2d_t(engine_t& engine, std::shared_ptr<tex_2d_t> const& resource, rtv_tex_2d_info_t const& info = {});

		~rtv_tex_2d_t() override;

	public:
		template <typename t_res>
		rtv_tex_2d_t(engine_t& engine, t_res const& resource, rtv_tex_2d_info_t const& info = {});
		
		// ------------------------------------------------

	public:
		auto init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER> override;

		void bind_for_rendering(D3D12_CPU_DESCRIPTOR_HANDLE const& bind_place) const override;

		auto shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER> override;

		// ------------------------------------------------

	public:
		auto get_resource_ptr() const->std::shared_ptr<tex_2d_t> const&;

		auto get_resource_ref() const->tex_2d_t&;

	private:
		std::shared_ptr<tex_2d_t> m_resource{};
		
		// ------------------------------------------------

	public:
		auto get_info() const->rtv_tex_2d_info_t const&;

	private:
		rtv_tex_2d_info_t m_info{};

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2
{
	template <typename t_res>
	rtv_tex_2d_t::rtv_tex_2d_t(engine_t& engine, t_res const& resource, rtv_tex_2d_info_t const& info /*= {}*/)
		: rtv_tex_2d_t
		{
			/*engine*/ engine,
			/*resource*/ std::make_shared<tex_2d_t>(engine, resource),
			/*info*/ info
		}
	{

	}
}
