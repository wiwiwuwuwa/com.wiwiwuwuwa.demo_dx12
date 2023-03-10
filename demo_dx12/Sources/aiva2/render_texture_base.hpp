#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct render_texture_base_t : public implements_t<render_texture_base_t, engine_object_t>
	{
		// ------------------------------------------------

	protected:
		render_texture_base_t(engine_t& engine);

		~render_texture_base_t() override;

		// ------------------------------------------------

	public:
		virtual void init_state_for_transition() const = 0;

		virtual void shut_state_for_transition() const = 0;

		virtual void init_state_for_uav() const = 0;

		// ------------------------------------------------

	public:
		auto get_cpu_descriptor_handle() const->D3D12_CPU_DESCRIPTOR_HANDLE;

	protected:
		winrt::com_ptr<ID3D12DescriptorHeap> m_descriptor_heap{};

		// ------------------------------------------------
	};
}
