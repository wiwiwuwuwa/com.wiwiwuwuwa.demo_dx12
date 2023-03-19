#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/rtv_eye.hpp>
#include <aiva2/dsv_tex_2d_info.hpp>
#include <aiva2/tex_2d_info.hpp>

namespace aiva2
{
	struct dsv_tex_2d_t final : public implements_t<dsv_tex_2d_t, rtv_eye_t>
	{
		// ------------------------------------------------

	public:
		dsv_tex_2d_t(engine_t& engine, std::shared_ptr<tex_2d_t> const& resource, dsv_tex_2d_info_t const& info = {});

		~dsv_tex_2d_t() override;

	public:
		template <typename t_res>
		dsv_tex_2d_t(engine_t& engine, t_res const& resource, dsv_tex_2d_info_t const& info = {});

		// ------------------------------------------------

	public:
		void init_for_rendering() const override;

		void shut_for_rendering() const override;

	private:
		void create_view() const;

		// ------------------------------------------------

	public:
		auto get_resource_ptr() const->std::shared_ptr<tex_2d_t> const&;

		auto get_resource_ref() const->tex_2d_t&;

	private:
		std::shared_ptr<tex_2d_t> m_resource{};

		// ------------------------------------------------

	public:
		auto get_info() const->dsv_tex_2d_info_t const&;

	private:
		dsv_tex_2d_info_t m_info{};

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2
{
	template <typename t_res>
	dsv_tex_2d_t::dsv_tex_2d_t(engine_t& engine, t_res const& resource, dsv_tex_2d_info_t const& info /*= {}*/)
		: dsv_tex_2d_t
		{
			/*engine*/ engine,
			/*resource*/ std::make_shared<tex_2d_t>(engine, resource),
			/*info*/ info
		}
	{

	}
}
