#pragma once
#include <pch.h>

#include <aiva/layer1/e_gpu_descriptor_heap_type.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	struct Engine;
	struct IGpuResourceView;
}

namespace aiva::utils
{
	template <typename, typename>
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
		enum class EDirtyFlags
		{
			None = 0,
			All = 1,
		};

		using CacheUpdaterType = aiva::utils::TCacheUpdater<EDirtyFlags, ShaderResourceDescriptor>;

	public:
		CacheUpdaterType& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};

	// ----------------------------------------------------
	// Resource Views

	public:
		std::shared_ptr<IGpuResourceView> ResourceView(std::string const& key) const;

		template <typename TGpuResourceView>
		std::shared_ptr<TGpuResourceView> ResourceView(std::string const& key) const;

		ShaderResourceDescriptor& ResourceView(std::string const& key, std::shared_ptr<IGpuResourceView> const& value);

	private:
		void OnResourceViewMarkedAsChanged();

	private:
		std::unordered_map<std::string, std::shared_ptr<IGpuResourceView>> mResourceViews{};

		std::unordered_map<std::string, boost::signals2::connection> mResourceConnections{};

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

		void RefreshDescriptorHeaps();

		void RefreshRootSignature();

	private:
		std::unordered_map<EGpuDescriptorHeapType, winrt::com_ptr<ID3D12DescriptorHeap>> mDescriptorHeaps{};

		winrt::com_ptr<ID3D12RootSignature> mRootSignature{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::ShaderResourceDescriptor> aiva::layer1::ShaderResourceDescriptor::Create(TArgs&&... args)
{
	return std::shared_ptr<ShaderResourceDescriptor>{new ShaderResourceDescriptor{ std::forward<TArgs>(args)... }};
}

template <typename TGpuResourceView>
std::shared_ptr<TGpuResourceView> aiva::layer1::ShaderResourceDescriptor::ResourceView(std::string const& key) const
{
	auto const basicResourceView = ResourceView(key);
	if (!basicResourceView) return {};

	auto const specificResourceView = std::dynamic_pointer_cast<TGpuResourceView>(basicResourceView);
	aiva::utils::Asserts::CheckBool(specificResourceView);

	return specificResourceView;
}
