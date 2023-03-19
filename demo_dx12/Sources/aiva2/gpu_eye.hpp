#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>

namespace aiva2
{
	struct gpu_eye_t : public implements_t<gpu_eye_t, engine_object_t>
	{
		// ------------------------------------------------

	protected:
		gpu_eye_t(engine_t& engine);

		~gpu_eye_t() override;

		// ------------------------------------------------

	public:
		virtual void init_for_rendering() const = 0;

		virtual void shut_for_rendering() const = 0;

		// ------------------------------------------------

	public:
		virtual auto get_cpu_descriptor_handle() const->D3D12_CPU_DESCRIPTOR_HANDLE = 0;

		virtual auto get_gpu_descriptor_handle() const->D3D12_GPU_DESCRIPTOR_HANDLE = 0;

		// ------------------------------------------------
	};
}