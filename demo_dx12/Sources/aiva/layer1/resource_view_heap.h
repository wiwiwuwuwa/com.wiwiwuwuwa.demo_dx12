#pragma once
#include <pch.h>

#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/i_object_engineable.h>
#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_cacheable.h>

namespace aiva::layer1
{
	struct AGraphicResourceView;
}

namespace aiva::layer1
{
	struct ResourceViewHeap final : public aiva::utils::AObject, public aiva::utils::IObjectCacheable, public aiva::layer1::IObjectEngineable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		ResourceViewHeap(EngineType const& engine);

		ResourceViewHeap(EngineType const& engine, EDescriptorHeapType const heapType);

	public:
		~ResourceViewHeap() override;

	// ----------------------------------------------------
	// Resource Type

	public:
		EDescriptorHeapType HeapType() const;

		void HeapType(EDescriptorHeapType const heapType);

	private:
		EDescriptorHeapType mHeapType{};

	// ----------------------------------------------------
	// Resource Views

	public:
		using ViewPtr = std::shared_ptr<AGraphicResourceView>;

		using ViewDict = std::map<std::string, ViewPtr>;

	public:
		ViewPtr GetView(std::string const& key) const;

		void SetView(std::string const& key, ViewPtr const& value);

		ViewDict const& GetViews() const;

	public:
		template <typename T>
		std::shared_ptr<T> GetView(std::string const& key) const;

	private:
		void ExecuteMarkAsChangedForSelf();

	private:
		ViewDict mViews{};

	// ----------------------------------------------------
	// Internal Resources

	public:
		winrt::com_ptr<ID3D12DescriptorHeap> const& InternalDescriptorHeap();

		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> const& InternalDescriptorHandles();

	private:
		void InitializeInternalResources();

		void TerminateInternalResources();

	private:
		void RefreshInternalResources();

		void ExecuteFlushForInternalResources();

		void RefreshInternalDescriptorHeap();

		void RefreshInternalDescriptorHandles();

	private:
		winrt::com_ptr<ID3D12DescriptorHeap> mInternalDescriptorHeap{};

		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> mInternalDescriptorHandles{};

	// ----------------------------------------------------
	// Descriptor Handles Utils

	public:
		std::optional<D3D12_CPU_DESCRIPTOR_HANDLE> InternalDescriptorHandle(std::string const& viewKey);

	// ----------------------------------------------------
	// Resource Barriers

	public:
		std::vector<D3D12_RESOURCE_BARRIER> PrepareBarriers(bool const active);

	// ----------------------------------------------------
	// Copying

	public:
		void CopyPropertiesFrom(ResourceViewHeap const& source);
	};
}

// --------------------------------------------------------

template <typename T>
std::shared_ptr<T> aiva::layer1::ResourceViewHeap::GetView(std::string const& key) const
{
	auto const basicView = GetView(key);
	if (!basicView) return {};

	auto const specificView = std::dynamic_pointer_cast<T>(basicView);
	aiva::utils::Asserts::CheckBool(specificView);

	return specificView;
}
