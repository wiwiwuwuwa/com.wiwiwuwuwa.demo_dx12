#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view.h>

namespace aiva::layer1
{
	struct ShaderStruct;
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
		using StructType = ShaderStruct;

	public:
		StructType& Struct() const;

	private:
		void InitializeStruct();

		void TerminateStruct();

	private:
		void Struct_OnChanged();

	private:
		std::shared_ptr<StructType> mStruct{};

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
