#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/gr_buffer_fwd.h>
#include <aiva/layer1/grv_srv_to_buffer_fwd.h>

namespace aiva::utils
{
	struct DictBuffer;
}

namespace aiva::layer1
{
	struct GrvSrvToBuffer final : public AGraphicResourceViewType
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	private:
		GrvSrvToBuffer(EngineType const& engine);

	public:
		~GrvSrvToBuffer() override;

	// ----------------------------------------------------
	// Aliases

	public:
		using BufferElementType = aiva::utils::DictBuffer;

		using BufferPointerType = std::shared_ptr<BufferElementType>;

		using ParentType = AGraphicResourceViewType;

		using ResourceType = GrBuffer;

		using ResourceTypeShared = std::shared_ptr<ResourceType>;

		using ResourceTypeWeak = std::weak_ptr<ResourceType>;

		using ThisType = GrvSrvToBuffer;

	// ----------------------------------------------------
	// Buffer

	public:
		BufferElementType& Buffer() const;

	private:
		void InitializeBuffer();

		void TerminateBuffer();

	private:
		void Buffer_OnChanged();

	private:
		BufferPointerType mBuffer{};

	// ----------------------------------------------------
	// Graphic Resource View: Internal Resource

	protected:
		ParentType::ResourceTypeShared CreateDefaultInternalResource() const override;

		void RefreshInternalResourceFromSelf(ParentType::ResourceTypeShared const& aivaResource, CacheFlagType const dirtyFlags) override;

	private:
		void RefreshInternalResourcePtr(ResourceTypeShared const& aivaResource, std::vector<std::byte> const& binaryData) const;

		void RefreshInternalResourceBin(ResourceTypeShared const& aivaResource, std::vector<std::byte> const& binaryData) const;

	// ----------------------------------------------------
	// Graphic Resource View: Metadata

	public:
		HeapTypeEnum HeapType() const override;

		ViewTypeEnum ViewType() const override;

	// ----------------------------------------------------
	// Graphic Resource View: Directx

	public:
		void CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) override;

		std::vector<D3D12_RESOURCE_BARRIER> CreateDirectxBarriers(bool const active) override;
	};
}
