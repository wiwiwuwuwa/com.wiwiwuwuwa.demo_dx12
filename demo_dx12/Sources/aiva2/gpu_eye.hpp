#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>

namespace aiva2
{
	struct gpu_eye_t : public implements_t<gpu_eye_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		using init_info_type = gpu_eye_init_info_t;

	protected:
		gpu_eye_t(engine_t& engine, init_info_type const& init_info);

		~gpu_eye_t() override;

		// ------------------------------------------------

	public:
		virtual void init_for_rendering() const = 0;

		virtual void shut_for_rendering() const = 0;

		// ------------------------------------------------
		
	public:
		auto get_cpu_descriptor_handle() const->D3D12_CPU_DESCRIPTOR_HANDLE;

		auto get_gpu_descriptor_handle() const->D3D12_GPU_DESCRIPTOR_HANDLE;

	private:
		void init_descriptor_heap(init_info_type const& init_info);

		void shut_descriptor_heap();

	private:
		winrt::com_ptr<ID3D12DescriptorHeap> m_descriptor_heap{};

		// ------------------------------------------------
	};
}