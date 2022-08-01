#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EGpuDescriptorHeapType : std::uint8_t;

	struct Engine;
	struct IGpuResourceView;
	struct ResourceViewHeap;
}

namespace aiva::utils
{
	enum class ECacheFlags : std::uint8_t;

	template <typename, typename = ECacheFlags>
	struct TCacheUpdater;
}

namespace aiva::layer1
{
	struct ResourceViewTable final : private boost::noncopyable, public std::enable_shared_from_this<ResourceViewTable>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<ResourceViewTable> Create(TArgs&&... args);

	private:
		ResourceViewTable(Engine const& engine);

	public:
		~ResourceViewTable();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Cache Refresh

	public:
		using CacheUpdaterType = aiva::utils::TCacheUpdater<ResourceViewTable>;

	public:
		CacheUpdaterType& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};

	// -------------------------------------------------------
	// Resource Heaps

	private:
		using ResourceHeapMap = std::unordered_map<EGpuDescriptorHeapType, std::shared_ptr<ResourceViewHeap>>;

	public:
		std::shared_ptr<ResourceViewHeap> GetResourceHeap(EGpuDescriptorHeapType const key) const;

		std::shared_ptr<ResourceViewHeap> GetOrAddResourceHeap(EGpuDescriptorHeapType const key);

		ResourceViewTable& SetResourceHeap(EGpuDescriptorHeapType const key, std::shared_ptr<ResourceViewHeap> const& value);

		ResourceHeapMap const& ResourceHeaps() const;

	private:
		void OnResourceHeapMarkedAsChanged();

	private:
		ResourceHeapMap mResourceHeaps{};

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

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::ResourceViewTable> aiva::layer1::ResourceViewTable::Create(TArgs&&... args)
{
	return std::shared_ptr<ResourceViewTable>{new ResourceViewTable{ std::forward<TArgs>(args)... }};
}
