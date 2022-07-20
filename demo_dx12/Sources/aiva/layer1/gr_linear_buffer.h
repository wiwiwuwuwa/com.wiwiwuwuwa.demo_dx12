#pragma once
#include <pch.h>

#include <aiva/layer1/e_gpu_resource_buffer_format.h>
#include <aiva/layer1/e_gpu_resource_memory_type.h>
#include <aiva/layer1/i_gpu_resource.h>
#include <aiva/utils/t_cache_refresh.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct GrLinearBuffer final : private boost::noncopyable, public std::enable_shared_from_this<GrLinearBuffer>, public aiva::layer1::IGpuResource
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<aiva::layer1::GrLinearBuffer> Create(TArgs&&... args);

	private:
		GrLinearBuffer(aiva::layer1::Engine const& engine);

	public:
		~GrLinearBuffer();

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// High-Level Data

	public:
		aiva::layer1::EGpuResourceMemoryType MemoryType() const;

		bool SupportsShaderAtomics() const;

		std::uint32_t Size() const;

		bool SupportsUnorderedAccess() const;

	private:
		aiva::layer1::EGpuResourceMemoryType mMemoryType{};

		bool mSupportsShaderAtomics{};

		std::uint32_t mSize{};

		bool mSupportsUnorderedAccess{};

	// ----------------------------------------------------
	// Cache Refresh

	public:
		enum class EDirtyFlags : std::uint8_t
		{
			None = 0,
			All = 1,
		};

	public:
		aiva::utils::TEvAction<EDirtyFlags>& OnFlushCompleted();

	private:
		aiva::utils::TCacheRefresh<EDirtyFlags> mCacheRefresh{ EDirtyFlags::All };

	// ----------------------------------------------------
	// Low-Level Data

	public:
		winrt::com_ptr<ID3D12Resource> const& InternalResource();

	private:
		void InitializeLowLevelData();

		void TerminateLowLevelData();
		
	private:
		void RefreshLowLevelData(EDirtyFlags const dirtyFlags);

	private:
		winrt::com_ptr<ID3D12Resource> mInternalResource{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrLinearBuffer> aiva::layer1::GrLinearBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<aiva::layer1::GrLinearBuffer>{new aiva::layer1::GrLinearBuffer{ std::forward<TArgs>(args)... }};
}
