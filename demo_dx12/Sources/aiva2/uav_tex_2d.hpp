#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/mat_eye.hpp>
#include <aiva2/uav_tex_2d_info.hpp>

namespace aiva2
{
	struct uav_tex_2d_t final : public implements_t<uav_tex_2d_t, mat_eye_t>
	{
		// ------------------------------------------------

	public:
		uav_tex_2d_t(engine_t& engine, std::shared_ptr<tex_2d_t> const& resource, uav_tex_2d_info_t const& info = {});

		~uav_tex_2d_t() override;

	private:
		void create_view() const;

		// ------------------------------------------------
		
	public:
		void init_for_rendering() const override;

		void shut_for_rendering() const override;

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