#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/grv_rtv_to_texture_2d_fwd.h>

namespace aiva::layer1
{
	struct GrvRtvToTexture2D final : public AGraphicResourceView
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		GrvRtvToTexture2D(EngineType const& engine);

	public:
		~GrvRtvToTexture2D() override;

	// ----------------------------------------------------
	// Metadata

	public:
		std::uint64_t MipLevel() const;

		GrvRtvToTexture2D& MipLevel(std::uint64_t const mipLevel);

	private:
		std::uint64_t mMipLevel{};

	// ----------------------------------------------------
	// Graphic Resource View: Internal Resource

	protected:
		std::shared_ptr<ResourceType> CreateDefaultInternalResource() const override;

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
