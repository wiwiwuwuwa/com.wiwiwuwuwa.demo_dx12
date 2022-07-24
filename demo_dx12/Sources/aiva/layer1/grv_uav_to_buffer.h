#pragma once
#include <pch.h>

#include <aiva/layer1/grv_uav_to_buffer_desc.h>
#include <aiva/layer1/i_gpu_resource_view.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	struct Engine;
	struct ShaderBuffer;
	struct ShaderStruct;
}

namespace aiva::layer1
{
	struct GrvUavToBuffer final : private boost::noncopyable, public std::enable_shared_from_this<GrvUavToBuffer>, public IGpuResourceView
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<GrvUavToBuffer> Create(TArgs&&... args);

	private:
		GrvUavToBuffer(Engine const& engine, GrvUavToBufferDesc const& desc);

	public:
		~GrvUavToBuffer();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Desc Data

	public:
		GrvUavToBufferDesc const& Desc() const;

		GrvUavToBuffer& Desc(GrvUavToBufferDesc const& desc);

	private:
		GrvUavToBufferDesc mDesc{};

	// ----------------------------------------------------
	// Buffer Data

	public:
		ShaderBuffer& Buffer() const;

		GrvUavToBuffer& ApplyBufferChanges();

	private:
		void RefreshBufferData(std::shared_ptr<const ShaderStruct> const& shaderStruct);

	private:
		std::shared_ptr<ShaderBuffer> mBuffer{};

	// ----------------------------------------------------
	// Updated Event

	public:
		aiva::utils::EvAction& OnInternalResourceUpdated();

	private:
		void RefreshInternalResourceUpdated(GrvUavToBufferDesc const& previousDesc, GrvUavToBufferDesc const& desiredDesc);

		void NotifyInternalResourceUpdated();

	private:
		aiva::utils::EvAction mOnInternalResourceUpdated{};

	// ----------------------------------------------------
	// Internal Data

	public:
		std::optional<D3D12_UNORDERED_ACCESS_VIEW_DESC> InternalResource() const;
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrvUavToBuffer> aiva::layer1::GrvUavToBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<GrvUavToBuffer>{new GrvUavToBuffer{ std::forward<TArgs>(args)... }};
}
