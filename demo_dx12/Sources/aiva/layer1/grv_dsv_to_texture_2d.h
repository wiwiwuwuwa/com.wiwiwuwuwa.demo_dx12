#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/grv_dsv_to_texture_2d_fwd.h>

namespace aiva::layer1
{
	struct GrvDsvToTexture2D final : public AGraphicResourceView
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		GrvDsvToTexture2D(EngineType const& engine);

	public:
		~GrvDsvToTexture2D() override;

	// ----------------------------------------------------
	// Metadata

	public:
		std::uint64_t MipLevel() const;

		GrvDsvToTexture2D& MipLevel(std::uint64_t const mipLevel);

	private:
		std::uint64_t mMipLevel{};

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
