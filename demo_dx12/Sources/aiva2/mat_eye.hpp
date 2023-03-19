#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>

namespace aiva2
{
	struct mat_eye_t : public implements_t<mat_eye_t, engine_object_t>
	{
		// ------------------------------------------------

	protected:
		mat_eye_t(engine_t& engine);

		~mat_eye_t() override;

		// ------------------------------------------------

	public:
		virtual void init_for_rendering() const = 0;

		virtual void shut_for_rendering() const = 0;

		// ------------------------------------------------
		
	public:
		auto get_cpu_descriptor_handle() const->D3D12_CPU_DESCRIPTOR_HANDLE;

		auto get_gpu_descriptor_handle() const->D3D12_GPU_DESCRIPTOR_HANDLE;

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
