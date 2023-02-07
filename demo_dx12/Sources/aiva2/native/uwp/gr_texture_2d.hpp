#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/core/object.hpp>
#include <aiva2/native/gr_texture_2d_params.hpp>

namespace aiva2::native
{
	struct gr_texture_2d_t final : public core::implements_t<gr_texture_2d_t, core::object_t>
	{
		// ------------------------------------------------
		// texture 2d data

	public:
		gr_texture_2d_t(engine_t& engine, gr_texture_2d_params_t const& params);

		gr_texture_2d_t(engine_t& engine, winrt::com_ptr<ID3D12Resource> const& resource);

		~gr_texture_2d_t() override;

	private:
		gr_texture_2d_t(engine_t& engine);

	public:
		auto get_engine() const->engine_t&;

		auto get_params() const->gr_texture_2d_params_t const&;

	private:
		void init_resource_from_params();

		void init_params_from_resource();

	private:
		engine_t& m_engine;

		gr_texture_2d_params_t m_params{};

		winrt::com_ptr<ID3D12Resource> m_resource{};

		// TODO: add support for barriers
	};
}
