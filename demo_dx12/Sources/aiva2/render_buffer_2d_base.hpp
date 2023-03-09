#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct render_buffer_2d_base_t : public implements_t<render_buffer_2d_base_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		render_buffer_2d_base_t(engine_t& engine);

		~render_buffer_2d_base_t() override;

		// ------------------------------------------------

	public:
		auto get_resource() const->std::shared_ptr<texture_2d_t> const&;
		
	protected:
		std::shared_ptr<texture_2d_t> m_resource{};

		// ------------------------------------------------

	public:
		auto get_cpu_descriptor_handle() const->D3D12_CPU_DESCRIPTOR_HANDLE;
		
	protected:
		winrt::com_ptr<ID3D12DescriptorHeap> m_descriptor_heap{};
	};
}