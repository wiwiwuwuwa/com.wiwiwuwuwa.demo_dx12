#pragma once
#include <pch.h>

#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/i_object_engineable.h>
#include <aiva/layer1/resource_view_heap_fwd.h>
#include <aiva/layer1/resource_view_table_fwd.h>
#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_changeable.h>

namespace aiva::layer1
{
	struct ResourceViewTable final : public aiva::utils::AObject, public aiva::utils::IObjectChangeable, public aiva::layer1::IObjectEngineable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		ResourceViewTable(EngineType const& engine);

	public:
		~ResourceViewTable() override;

	// -------------------------------------------------------
	// Resource Heaps

	private:
		using HeapDict = std::unordered_map<EDescriptorHeapType, ResourceViewHeapTypeShared>;

	public:
		ResourceViewHeapTypeShared GetResourceHeap(EDescriptorHeapType const key) const;

		ResourceViewHeapTypeShared GetOrAddResourceHeap(EDescriptorHeapType const key);

		ResourceViewTable& SetResourceHeap(EDescriptorHeapType const key, ResourceViewHeapTypeShared const& value);

		HeapDict const& ResourceHeaps() const;

	private:
		void ResourceHeap_OnMarkedAsChanged();

	private:
		HeapDict mResourceHeaps{};

	// ----------------------------------------------------
	// Internal Resources

	public:
		std::vector<winrt::com_ptr<ID3D12DescriptorHeap>> InternalResource() const;

	// ----------------------------------------------------
	// Resource Barriers

	public:
		std::vector<D3D12_RESOURCE_BARRIER> PrepareBarriers(bool const active) const;

	// ----------------------------------------------------
	// Copying

	public:
		void CopyPropertiesFrom(ResourceViewTable const& source);
	};
}
