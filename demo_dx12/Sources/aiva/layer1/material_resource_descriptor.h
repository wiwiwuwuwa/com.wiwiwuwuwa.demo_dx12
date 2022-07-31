#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct Engine;
	struct ResourceViewTable;
}

namespace aiva::utils
{
	enum class ECacheFlags : std::uint8_t;

	template <typename, typename = ECacheFlags>
	struct TCacheUpdater;
}

namespace aiva::layer1
{
	struct MaterialResourceDescriptor final : private boost::noncopyable, public std::enable_shared_from_this<MaterialResourceDescriptor>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<MaterialResourceDescriptor> Create(TArgs&&... args);

	private:
		MaterialResourceDescriptor(Engine const& engine);

	public:
		~MaterialResourceDescriptor();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Cache Refresh

	public:
		using CacheUpdaterType = aiva::utils::TCacheUpdater<MaterialResourceDescriptor>;

	public:
		CacheUpdaterType& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};

	// ----------------------------------------------------
	// Resource Table

	public:
		ResourceViewTable& ResourceTable() const;

	private:
		void InitializeResourceTable();

		void TerminateResourceTable();

	private:
		void OnResourceTableMarkedAsChanged();

	private:
		std::shared_ptr<ResourceViewTable> mResourceTable{};

	// ----------------------------------------------------
	// Internal Resources Data

	public:
		std::vector<winrt::com_ptr<ID3D12DescriptorHeap>> InternalDescriptorHeaps() const;

		winrt::com_ptr<ID3D12RootSignature> InternalRootSignature() const;

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

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::MaterialResourceDescriptor> aiva::layer1::MaterialResourceDescriptor::Create(TArgs&&... args)
{
	return std::shared_ptr<MaterialResourceDescriptor>{new MaterialResourceDescriptor{ std::forward<TArgs>(args)... }};
}
