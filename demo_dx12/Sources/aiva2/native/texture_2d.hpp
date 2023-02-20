#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/native/graphic_resource.hpp>
#include <aiva2/native/texture_2d_params.hpp>

namespace aiva2::native
{
	struct texture_2d_t final : public core::implements_t<texture_2d_t, graphic_resource_t>
	{
		// ------------------------------------------------
		// texture 2d data

	public:
		texture_2d_t(engine_t& engine, texture_2d_params_t const& params);

		texture_2d_t(engine_t& engine, winrt::com_ptr<ID3D12Resource> const& resource);

		~texture_2d_t() override;

	public:
		auto get_params() const->texture_2d_params_t const&;

	private:
		void init_resource_from_params();

		void init_params_from_resource();

	private:
		texture_2d_params_t m_params{};
	};
}
