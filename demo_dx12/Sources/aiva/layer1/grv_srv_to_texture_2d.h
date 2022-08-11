#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view.h>

namespace aiva::layer1
{
	struct GrvSrvToTexture2D final : AGraphicResourceView
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		GrvSrvToTexture2D(EngineType const& engine);

	public:
		~GrvSrvToTexture2D() override;

	// ----------------------------------------------------
	// Graphic Resource View: Metadata

	public:
		EDescriptorHeapType HeapType() const override;

		EResourceViewType ViewType() const override;

	// ----------------------------------------------------
	// Graphic Resource View: DirectX

	public:
		void CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) override;

		std::vector<D3D12_RESOURCE_BARRIER> CreateDirectxBarriers(bool const active) override;
	};
}
