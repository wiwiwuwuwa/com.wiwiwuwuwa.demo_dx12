#pragma once
#include <pch.h>

#include <aiva/layer1/grv_srv_to_buffer_desc.h>
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
	struct GrvSrvToBuffer final : private boost::noncopyable, public std::enable_shared_from_this<GrvSrvToBuffer>, public IGpuResourceView
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<GrvSrvToBuffer> Create(TArgs&&... args);

	private:
		GrvSrvToBuffer(Engine const& engine, GrvSrvToBufferDesc const& desc);

	public:
		~GrvSrvToBuffer();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Desc Data

	public:
		GrvSrvToBufferDesc const& Desc() const;

		GrvSrvToBuffer& Desc(GrvSrvToBufferDesc const& desc);

	private:
		GrvSrvToBufferDesc mDesc{};

	// ----------------------------------------------------
	// Buffer Data

	public:
		ShaderBuffer& Buffer() const;

		GrvSrvToBuffer& ApplyBufferChanges();

	private:
		void RefreshBufferData(std::shared_ptr<const ShaderStruct> const& shaderStruct);

	private:
		std::shared_ptr<ShaderBuffer> mBuffer{};

	// ----------------------------------------------------
	// Updated Event

	public:
		aiva::utils::EvAction& OnInternalResourceUpdated();

	private:
		void RefreshInternalResourceUpdated(GrvSrvToBufferDesc const& previousDesc, GrvSrvToBufferDesc const& desiredDesc);

		void NotifyInternalResourceUpdated();

	private:
		aiva::utils::EvAction mOnInternalResourceUpdated{};

	// ----------------------------------------------------
	// Internal Data

	public:
		std::optional<D3D12_SHADER_RESOURCE_VIEW_DESC> InternalResource() const;
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrvSrvToBuffer> aiva::layer1::GrvSrvToBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<GrvSrvToBuffer>{new GrvSrvToBuffer{ std::forward<TArgs>(args)... }};
}
