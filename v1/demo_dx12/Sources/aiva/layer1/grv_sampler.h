#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/grv_sampler_fwd.h>

namespace aiva::layer1
{
	struct GrvSampler final : public AGraphicResourceView
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		GrvSampler(EngineType const& engine);

	public:
		~GrvSampler() override;

	// ----------------------------------------------------
	// Graphic Resource View: Metadata

	public:
		EDescriptorHeapType HeapType() const override;

		EResourceViewType ViewType() const override;

	// ----------------------------------------------------
	// Graphic Resource View: Directx

	public:
		void CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) override;

		std::vector<D3D12_RESOURCE_BARRIER> CreateDirectxBarriers(bool const active) override;
	};
}
