#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_eye.hpp>
#include <aiva2/uav_tex_2d_info.hpp>

namespace aiva2
{
	struct uav_tex_2d_t final : public implements_t<uav_tex_2d_t, gpu_eye_t>
	{
		// ------------------------------------------------

	public:
		using resource_type = tex_2d_t;

		using info_type = uav_tex_2d_info_t;

	public:
		uav_tex_2d_t(engine_t& engine, std::shared_ptr<tex_2d_t> const& resource, uav_tex_2d_info_t const& info = {});

		~uav_tex_2d_t() override;

	public:
		template <typename t_res>
		uav_tex_2d_t(engine_t& engine, t_res const& resource, uav_tex_2d_info_t const& info = {});

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
		auto get_info() const->uav_tex_2d_info_t const&;

	private:
		uav_tex_2d_info_t m_info{};

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2
{
	template <typename t_res>
	uav_tex_2d_t::uav_tex_2d_t(engine_t& engine, t_res const& resource, uav_tex_2d_info_t const& info /*= {}*/)
		: uav_tex_2d_t
		{
			/*engine*/ engine,
			/*resource*/ std::make_shared<tex_2d_t>(engine, resource),
			/*info*/ info
		}
	{

	}
}
