#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view.h>

namespace aiva::layer1
{
	struct ShaderBuffer;
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
		using BufferType = ShaderBuffer;

	public:
		BufferType& Buffer() const;

	private:
		void InitializeBuffer();

		void TerminateBuffer();

	private:
		void Buffer_OnChanged();

	private:
		std::shared_ptr<BufferType> mBuffer{};

	// ----------------------------------------------------
	// Graphic Resource View: Internal Resource

	protected:
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
