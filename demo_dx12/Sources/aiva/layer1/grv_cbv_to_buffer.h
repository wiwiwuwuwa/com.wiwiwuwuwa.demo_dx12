#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view.h>

namespace aiva::layer1
{
	struct GrBuffer;
}

namespace aiva::utils
{
	struct DictBuffer;
	struct DictStruct;
}

namespace aiva::layer1
{
	struct GrvCbvToBuffer final : public AGraphicResourceView
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	private:
		GrvCbvToBuffer(EngineType const& engine);

	public:
		~GrvCbvToBuffer() override;

	// ----------------------------------------------------
	// Buffer

	public:
		using BufferElementType = aiva::utils::DictBuffer;

		using BufferPointerType = std::shared_ptr<BufferElementType>;

		using StructElementType = aiva::utils::DictStruct;

		using StructPointerType = std::shared_ptr<StructElementType>;

	public:
		StructElementType& Struct() const;

	private:
		void InitializeStruct();

		void TerminateStruct();

	private:
		void Struct_OnChanged();

	private:
		StructPointerType mStruct{};

	// ----------------------------------------------------
	// Graphic Resource View: Internal Resource

	protected:
		std::shared_ptr<ResourceType> CreateDefaultInternalResource() const override;

		void RefreshInternalResourceFromSelf(std::shared_ptr<ResourceType> const& aivaResource, EGrvCacheFlags const dirtyFlags) override;

	private:
		void RefreshInternalResourcePtr(std::shared_ptr<GrBuffer> const& aivaResource, std::vector<std::byte> const& binaryData) const;

		void RefreshInternalResourceBin(std::shared_ptr<GrBuffer> const& aivaResource, std::vector<std::byte> const& binaryData) const;

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
