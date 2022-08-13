#pragma once
#include <pch.h>

#include <aiva/layer1/e_rvt_cache_flags.h>
#include <aiva/layer1/i_object_engineable.h>
#include <aiva/layer1/resource_view_table_fwd.h>
#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_cacheable.h>

namespace aiva::layer1
{
	struct MaterialResourceDescriptor final : public aiva::utils::AObject, public aiva::utils::IObjectCacheable, public IObjectEngineable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;
		
	protected:
		MaterialResourceDescriptor(EngineType const& engine);

	public:
		~MaterialResourceDescriptor() override;

	// ----------------------------------------------------
	// Resource Table

	public:
		ResourceViewTable& ResourceTable() const;

	private:
		void InitializeResourceTable();

		void TerminateResourceTable();

	private:
		void OnResourceTableMarkedAsChanged(ERvtCacheFlags const dirtyFlags);

	private:
		std::shared_ptr<ResourceViewTable> mResourceTable{};

	// ----------------------------------------------------
	// Internal Resources Data

	public:
		std::vector<winrt::com_ptr<ID3D12DescriptorHeap>> InternalDescriptorHeaps();

		winrt::com_ptr<ID3D12RootSignature> InternalRootSignature();

	private:
		void InitializeInternalResources();

		void TerminateInternalResources();

	private:
		void RefreshInternalResources();

		void RefreshRootSignature();

	private:
		winrt::com_ptr<ID3D12RootSignature> mRootSignature{};

	// ----------------------------------------------------
	// Resource Barriers

	public:
		std::vector<D3D12_RESOURCE_BARRIER> PrepareBarriers(bool const active) const;

	// ----------------------------------------------------
	// Copying

	public:
		void CopyPropertiesFrom(MaterialResourceDescriptor const& source);
	};
}
