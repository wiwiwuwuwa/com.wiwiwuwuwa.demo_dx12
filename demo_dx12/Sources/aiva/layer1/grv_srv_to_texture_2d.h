#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/gr_texture_2d_fwd.h>
#include <aiva/layer1/grv_srv_to_texture_2d_fwd.h>

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
	// Aliases

	public:
		using ParentType = AGraphicResourceViewType;

		using ResourceType = GrTexture2DType;

		using ResourceTypeShared = std::shared_ptr<ResourceType>;

		using ResourceTypeWeak = std::weak_ptr<ResourceType>;

		using ThisType = GrvSrvToTexture2DType;

	// ----------------------------------------------------
	// Graphic Resource View: Internal Resource

	protected:
		ParentType::ResourceTypeShared CreateDefaultInternalResource() const override;

	// ----------------------------------------------------
	// Graphic Resource View: Metadata

	public:
		HeapTypeEnum HeapType() const override;

		ViewTypeEnum ViewType() const override;

	// ----------------------------------------------------
	// Graphic Resource View: DirectX

	public:
		void CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) override;

		std::vector<D3D12_RESOURCE_BARRIER> CreateDirectxBarriers(bool const active) override;
	};
}
