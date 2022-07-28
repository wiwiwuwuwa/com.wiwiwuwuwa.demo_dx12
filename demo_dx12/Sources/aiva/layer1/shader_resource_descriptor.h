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
	struct ShaderResourceDescriptor final : private boost::noncopyable, public std::enable_shared_from_this<ShaderResourceDescriptor>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<ShaderResourceDescriptor> Create(TArgs&&... args);

	private:
		ShaderResourceDescriptor(Engine const& engine);

	public:
		~ShaderResourceDescriptor();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Cache Refresh

	public:
		using CacheUpdaterType = aiva::utils::TCacheUpdater<ShaderResourceDescriptor>;

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
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::ShaderResourceDescriptor> aiva::layer1::ShaderResourceDescriptor::Create(TArgs&&... args)
{
	return std::shared_ptr<ShaderResourceDescriptor>{new ShaderResourceDescriptor{ std::forward<TArgs>(args)... }};
}
