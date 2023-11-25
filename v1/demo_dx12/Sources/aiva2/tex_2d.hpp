#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_res.hpp>
#include <aiva2/implements.hpp>
#include <aiva2/tex_2d_info.hpp>

namespace aiva2
{
	struct tex_2d_t final : public implements_t<tex_2d_t, gpu_res_t>
	{
		// ------------------------------------------------

	public:
		tex_2d_t(engine_t& engine, tex_2d_info_t const& info);

		tex_2d_t(engine_t& engine, winrt::com_ptr<ID3D12Resource> const& resource);

		~tex_2d_t() override;

		// ------------------------------------------------

	public:
		auto get_info() const->tex_2d_info_t const&;

	private:
		void init_resource_from_info();

		void init_info_from_resource();

	private:
		tex_2d_info_t m_info{};

		// ------------------------------------------------
	};
}
