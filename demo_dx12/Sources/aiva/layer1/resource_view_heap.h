#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EGpuDescriptorHeapType : std::uint8_t;

	struct Engine;
	struct IGpuResourceView;
}

namespace aiva::utils
{
	enum class ECacheFlags : std::uint8_t;

	template <typename, typename = ECacheFlags>
	struct TCacheUpdater;
}

namespace aiva::layer1
{
	struct ResourceViewHeap final : private boost::noncopyable, public std::enable_shared_from_this<ResourceViewHeap>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<ResourceViewHeap> Create(TArgs&&... args);

	private:
		ResourceViewHeap(Engine const& engine);

		ResourceViewHeap(Engine const& engine, EGpuDescriptorHeapType const resourceType);

	public:
		~ResourceViewHeap();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Cache Refresh

	public:
		using CacheUpdaterType = aiva::utils::TCacheUpdater<ResourceViewHeap>;

	public:
		CacheUpdaterType& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};

	// ----------------------------------------------------
	// Resource Type

	public:
		EGpuDescriptorHeapType ResourceType() const;

		ResourceViewHeap& ResourceType(EGpuDescriptorHeapType const resourceType);

	private:
		EGpuDescriptorHeapType mResourceType{};

	// ----------------------------------------------------
	// Resource Views

	private:
		using ResourceViewsMap = std::map<std::string, std::shared_ptr<IGpuResourceView>>;

	public:
		std::shared_ptr<IGpuResourceView> ResourceView(std::string const& key) const;

		template <typename TGpuResourceView>
		std::shared_ptr<TGpuResourceView> ResourceView(std::string const& key) const;

		ResourceViewHeap& ResourceView(std::string const& key, std::shared_ptr<IGpuResourceView> const& value);

		ResourceViewsMap const& ResourceViews() const;

	private:
		void OnResourceViewMarkedAsChanged();

	private:
		ResourceViewsMap mResourceViews{};

	// ----------------------------------------------------
	// Internal Resources

	public:
		winrt::com_ptr<ID3D12DescriptorHeap> const& InternalDescriptorHeap() const;

		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> const& InternalDescriptorHandles() const;

	private:
		void InitializeInternalResources();

		void TerminateInternalResources();

	private:
		void RefreshInternalResources();

		void RefreshInternalDescriptorHeap();

		void RefreshInternalDescriptorHandles();

	private:
		winrt::com_ptr<ID3D12DescriptorHeap> mInternalDescriptorHeap{};

		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> mInternalDescriptorHandles{};

	// ----------------------------------------------------
	// Descriptor Handles Utils

	public:
		std::optional<D3D12_CPU_DESCRIPTOR_HANDLE> InternalDescriptorHandle(std::string const& viewKey) const;

	// ----------------------------------------------------
	// Resource Barriers

	public:
		std::vector<D3D12_RESOURCE_BARRIER> PrepareBarriers(bool const active) const;
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::ResourceViewHeap> aiva::layer1::ResourceViewHeap::Create(TArgs&&... args)
{
	return std::shared_ptr<ResourceViewHeap>{new ResourceViewHeap{ std::forward<TArgs>(args)... }};
}

template <typename TGpuResourceView>
std::shared_ptr<TGpuResourceView> aiva::layer1::ResourceViewHeap::ResourceView(std::string const& key) const
{
	auto const basicResourceView = ResourceView(key);
	if (!basicResourceView) return {};

	auto const specificResourceView = std::dynamic_pointer_cast<TGpuResourceView>(basicResourceView);
	aiva::utils::Asserts::CheckBool(specificResourceView);

	return specificResourceView;
}
