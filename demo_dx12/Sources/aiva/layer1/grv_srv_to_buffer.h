#pragma once
#include <pch.h>

#include <aiva/layer1/grv_srv_to_buffer_desc.h>
#include <aiva/layer1/i_gpu_resource_view.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	enum class EGpuDescriptorHeapType : std::uint8_t;
	enum class EGpuResourceViewType : std::uint8_t;

	struct Engine;
	struct ShaderBuffer;
	struct ShaderStruct;
}

namespace aiva::utils
{
	enum class ECacheFlags : std::uint8_t;

	template <typename, typename = ECacheFlags>
	struct TCacheUpdater;
}

namespace aiva::layer1
{
	struct GrvSrvToBuffer final : private boost::noncopyable, public std::enable_shared_from_this<GrvSrvToBuffer>, public IGpuResourceView
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<GrvSrvToBuffer> Create(TArgs&&... args);

	private:
		GrvSrvToBuffer(Engine const& engine);

	public:
		~GrvSrvToBuffer();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Cache Refresh

	public:
		using CacheUpdaterType = aiva::utils::TCacheUpdater<GrvSrvToBuffer>;

	public:
		CacheUpdaterType& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};

	// ----------------------------------------------------
	// IGpuResourceView

	public:
		EGpuDescriptorHeapType HeapType() const override;

		EGpuResourceViewType ViewType() const override;

		void CreateView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) const override;

		std::vector<D3D12_RESOURCE_BARRIER> PrepareBarriers(bool const active) const override;

		aiva::utils::TEvAction<aiva::utils::ECacheFlags>& OnMarkAsChanged() override;

	// ----------------------------------------------------
	// Desc Data

	public:
		std::optional<GrvSrvToBufferDesc> const& Desc() const;

		GrvSrvToBuffer& Desc(std::optional<GrvSrvToBufferDesc> const& desc);

	private:
		void OnDescResourceMarkedAsChanged() const;

	private:
		std::optional<GrvSrvToBufferDesc> mDesc{};

	// ----------------------------------------------------
	// Buffer Data

	public:
		ShaderBuffer& Buffer() const;

	private:
		void InitializeBuffer();

		void TerminateBuffer();

	private:
		void OnBufferMarkedAsChanged() const;

	private:
		std::shared_ptr<ShaderBuffer> mBuffer{};

	// ----------------------------------------------------
	// Internal Data

	public:
		std::optional<D3D12_SHADER_RESOURCE_VIEW_DESC> InternalResource() const;

	private:
		void InitializeInternalResources();

		void TerminateInternalResources();

	private:
		void RefreshInternalResources();
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrvSrvToBuffer> aiva::layer1::GrvSrvToBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<GrvSrvToBuffer>{new GrvSrvToBuffer{ std::forward<TArgs>(args)... }};
}
