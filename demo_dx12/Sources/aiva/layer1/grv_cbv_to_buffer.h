#pragma once
#include <pch.h>

#include <aiva/layer1/grv_cbv_to_buffer_desc.h>
#include <aiva/layer1/i_gpu_resource_view.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct GrvCbvToBuffer final : private boost::noncopyable, public std::enable_shared_from_this<GrvCbvToBuffer>, public IGpuResourceView
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<GrvCbvToBuffer> Create(TArgs&&... args);

	private:
		GrvCbvToBuffer(Engine const& engine, GrvCbvToBufferDesc const& desc);

	public:
		~GrvCbvToBuffer();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// High-Level Data

	public:
		GrvCbvToBufferDesc const Desc() const;

		GrvCbvToBuffer& Desc(GrvCbvToBufferDesc const& desc);

	private:
		void OnDescInternalResourceUpdated();

	private:
		GrvCbvToBufferDesc mDesc{};

	// ----------------------------------------------------
	// Low-Level Data

	public:
		std::optional<D3D12_CONSTANT_BUFFER_VIEW_DESC> InternalResource();

		aiva::utils::EvAction& OnInternalResourceUpdated();

	private:
		void NotifyInternalResourceUpdated();

	private:
		aiva::utils::EvAction mOnInternalResourceUpdated;
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrvCbvToBuffer> aiva::layer1::GrvCbvToBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<GrvCbvToBuffer>{new GrvCbvToBuffer{ std::forward<TArgs>(args)... }};
}
