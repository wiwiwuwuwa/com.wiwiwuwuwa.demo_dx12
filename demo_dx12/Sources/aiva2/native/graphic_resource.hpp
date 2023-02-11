#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/native/engine_object.hpp>
#include <aiva2/native/graphic_resource_states.hpp>

namespace aiva2::native
{
	struct graphic_resource_t : public core::implements_t<graphic_resource_t, engine_object_t>
	{
		// ------------------------------------------------
		// graphic resource

	protected:
		graphic_resource_t(engine_t& engine);

		graphic_resource_t(engine_t& engine, winrt::com_ptr<ID3D12Resource> const& resource);

		~graphic_resource_t() override;

	protected:
		winrt::com_ptr<ID3D12Resource> m_resource{};

		graphic_resource_states_t m_states{};
	};
}
