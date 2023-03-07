#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_res_base.hpp>
#include <aiva2/implements.hpp>
#include <aiva2/texture_2d_info.hpp>

namespace aiva2
{
	struct texture_2d_t final : public implements_t<texture_2d_t, gpu_res_base_t>
	{
		// ------------------------------------------------

	public:
		texture_2d_t(engine_t& engine, texture_2d_info_t const& info);

		texture_2d_t(engine_t& engine, winrt::com_ptr<ID3D12Resource> const& resource);

		~texture_2d_t() override;

		// ------------------------------------------------

	public:
		auto get_info() const->texture_2d_info_t const&;

	private:
		void init_resource_from_info();

		void init_info_from_resource();

	private:
		texture_2d_info_t m_info{};

		// ------------------------------------------------
	};
}
