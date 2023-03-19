#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_eye.hpp>

namespace aiva2
{
	struct mat_eye_t : public implements_t<mat_eye_t, gpu_eye_t>
	{
		// ------------------------------------------------

	protected:
		mat_eye_t(engine_t& engine);

		~mat_eye_t() override;

		// ------------------------------------------------
		
	public:
		auto get_cpu_descriptor_handle() const->D3D12_CPU_DESCRIPTOR_HANDLE override;

		auto get_gpu_descriptor_handle() const->D3D12_GPU_DESCRIPTOR_HANDLE override;

	protected:
		auto get_descriptor_heap_ptr() const->winrt::com_ptr<ID3D12DescriptorHeap> const&;

		auto get_descriptor_heap_ref() const->ID3D12DescriptorHeap&;

	private:
		void init_descriptor_heap();

		void shut_descriptor_heap();

	private:
		winrt::com_ptr<ID3D12DescriptorHeap> m_descriptor_heap{};

		// ------------------------------------------------
	};
}
