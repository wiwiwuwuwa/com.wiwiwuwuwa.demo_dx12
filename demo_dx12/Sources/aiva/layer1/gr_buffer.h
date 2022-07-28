#pragma once
#include <pch.h>

#include <aiva/layer1/i_gpu_resource.h>
#include <aiva/layer1/gr_buffer_desc.h>
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
	struct GrBuffer final : private boost::noncopyable, public std::enable_shared_from_this<GrBuffer>, public aiva::layer1::IGpuResource
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<GrBuffer> Create(TArgs&&... args);

	private:
		GrBuffer(Engine const& engine);

		GrBuffer(Engine const& engine, GrBufferDesc const& desc);

		GrBuffer(Engine const& engine, winrt::com_ptr<ID3D12Resource> const& resource);

	public:
		~GrBuffer();

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// Cache Refresh

	public:
		enum class EDirtyFlags
		{
			None = 0,
			All = 1,
		};

		using CacheUpdaterType = aiva::utils::TCacheUpdater<EDirtyFlags, GrBuffer>;

	public:
		CacheUpdaterType& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};

	// ----------------------------------------------------
	// High-Level Data

	public:
		std::optional<GrBufferDesc> const& Desc() const;

		GrBuffer& Desc(std::optional<GrBufferDesc> const& desc);

	private:
		GrBuffer& Desc(std::optional<GrBufferDesc> const& desc, bool const markAsChanged);

	private:
		std::optional<GrBufferDesc> mDesc{};

	// ----------------------------------------------------
	// Low-Level Data

	public:
		winrt::com_ptr<ID3D12Resource> const InternalResource();

	private:
		GrBuffer& InternalResource(winrt::com_ptr<ID3D12Resource> const& resource);

	private:
		void InitializeInternalResources();

		void TerminateInternalResources();

	private:
		void RefreshInternalResources();

		static winrt::com_ptr<ID3D12Resource> CreateInternalResource(Engine const& engine, GrBufferDesc const& desc);

	private:
		winrt::com_ptr<ID3D12Resource> mInternalResource{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrBuffer> aiva::layer1::GrBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<GrBuffer>{new GrBuffer{ std::forward<TArgs>(args)... }};
}
