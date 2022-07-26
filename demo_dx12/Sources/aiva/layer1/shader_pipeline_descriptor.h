#pragma once
#include <pch.h>

#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::utils
{
	template <typename, typename>
	struct TCacheUpdater;
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

		using CacheUpdaterType = aiva::utils::TCacheUpdater<EDirtyFlags, ShaderPipelineDescriptor>;

	public:
		CacheUpdaterType& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::ShaderPipelineDescriptor> aiva::layer1::ShaderPipelineDescriptor::Create(TArgs&&... args)
{
	return std::shared_ptr<ShaderPipelineDescriptor>{new ShaderPipelineDescriptor{ std::forward<TArgs>(args)... }};
}
