#pragma once
#include <pch.h>

#include <aiva/layer1/e_gpu_descriptor_heap_type.h>
#include <aiva/utils/ev_action.h>
#include <aiva/utils/t_cache_refresh.h>

namespace aiva::layer1
{
	struct Engine;
	struct IGpuResourceView;
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

	public:
		aiva::utils::TCacheRefresh<EDirtyFlags>& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<aiva::utils::TCacheRefresh<EDirtyFlags>> mCacheUpdater{};

	// ----------------------------------------------------
	// Resource Views

	public:
		std::shared_ptr<IGpuResourceView> ResourceView(std::string const& key) const;

		ShaderResourceDescriptor& ResourceView(std::string const& key, std::shared_ptr<IGpuResourceView> const& value);

	private:
		void OnResourceViewUpdated();

	private:
		std::unordered_map<std::string, std::shared_ptr<IGpuResourceView>> mResourceViews{};

	// ----------------------------------------------------
	// Internal Resources Data

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

	// ----------------------------------------------------
	// Internal Resources Events

	public:
		aiva::utils::EvAction& OnInternalResourceUpdated();

	private:
		aiva::utils::EvAction mOnInternalResourceUpdated{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::ShaderResourceDescriptor> aiva::layer1::ShaderResourceDescriptor::Create(TArgs&&... args)
{
	return std::shared_ptr<ShaderResourceDescriptor>{new ShaderResourceDescriptor{ std::forward<TArgs>(args)... }};
}
