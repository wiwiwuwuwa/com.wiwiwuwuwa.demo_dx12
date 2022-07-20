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
	struct GrTextureBuffer2D final : private boost::noncopyable, public std::enable_shared_from_this<GrTextureBuffer2D>, public aiva::layer1::IGpuResource
	{
		// ----------------------------------------------------
		// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<aiva::layer1::GrTextureBuffer2D> Create(TArgs&&... args);

	private:
		GrTextureBuffer2D(aiva::layer1::Engine const& engine);

	public:
		~GrTextureBuffer2D();

	private:
		aiva::layer1::Engine const& mEngine;

		// ----------------------------------------------------
		// High-Level Data

	public:
		bool SupportsShaderAtomics() const;

		aiva::layer1::GrTextureBuffer2D& SupportsShaderAtomics(bool const value);

		std::uint64_t Width() const;

		aiva::layer1::GrTextureBuffer2D& Width(std::uint64_t const width);

		std::uint64_t Height() const;

		aiva::layer1::GrTextureBuffer2D& Height(std::uint64_t const height);

		bool SupportsMipMapping() const;

		aiva::layer1::GrTextureBuffer2D& SupportsMipMapping(bool const value);

		aiva::layer1::EGpuResourceBufferFormat BufferFormat() const;

		aiva::layer1::GrTextureBuffer2D& BufferFormat(aiva::layer1::EGpuResourceBufferFormat const bufferFormat);

		bool SupportsRenderTarget() const;

		aiva::layer1::GrTextureBuffer2D& SupportsRenderTarget(bool const value);

		bool SupportsDepthStencil() const;

		aiva::layer1::GrTextureBuffer2D& SupportsDepthStencil(bool const value);

		bool SupportsUnorderedAccess() const;

		aiva::layer1::GrTextureBuffer2D& SupportsUnorderedAccess(const bool value);

	private:
		bool mSupportsShaderAtomics{};

		std::uint64_t mWidth{};

		std::uint64_t mHeight{};

		bool mSupportsMipMapping{};

		aiva::layer1::EGpuResourceBufferFormat mBufferFormat{};

		bool mSupportsRenderTarget{};

		bool mSupportsDepthStencil{};

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
std::shared_ptr<aiva::layer1::GrTextureBuffer2D> aiva::layer1::GrTextureBuffer2D::Create(TArgs&&... args)
{
	return std::shared_ptr<aiva::layer1::GrTextureBuffer2D>{new aiva::layer1::GrTextureBuffer2D{ std::forward<TArgs>(args)... }};
}
