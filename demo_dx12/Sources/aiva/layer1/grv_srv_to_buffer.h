#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view.h>

namespace aiva::utils
{
	struct DictBuffer;
}

namespace aiva::layer1
{
	struct GrvSrvToBuffer final : public AGraphicResourceView
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
	// Buffer

	public:
		using BufferElementType = aiva::utils::DictBuffer;

		using BufferPointerType = std::shared_ptr<BufferElementType>;

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
		std::shared_ptr<ResourceType> CreateDefaultInternalResource() const override;

		void RefreshInternalResourceFromSelf(std::shared_ptr<ResourceType> const& aivaResource) override;

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
