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
		virtual auto init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER> = 0;

		virtual void bind_for_rendering(D3D12_CPU_DESCRIPTOR_HANDLE const& bind_place) const = 0;

		virtual auto shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER> = 0;

		// ------------------------------------------------
	};
}