#pragma once
#include <pch.h>

#include <aiva/utils/ev_action.h>
#include <aiva/utils/t_cache_refresh.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct ShaderPipelineDescriptor final : private boost::noncopyable, public std::enable_shared_from_this<ShaderPipelineDescriptor>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<ShaderPipelineDescriptor> Create(TArgs&&... args);

	private:
		ShaderPipelineDescriptor(Engine const& engine);

	public:
		~ShaderPipelineDescriptor();

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
	// Internal Resources Data

	private:
		void InitializeInternalResources();

		void TerminateInternalResources();

	private:
		void NotifyInternalResourcesUpdated();

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
std::shared_ptr<aiva::layer1::ShaderPipelineDescriptor> aiva::layer1::ShaderPipelineDescriptor::Create(TArgs&&... args)
{
	return std::shared_ptr<ShaderPipelineDescriptor>{new ShaderPipelineDescriptor{ std::forward<TArgs>(args)... }};
}
