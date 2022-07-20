#pragma once
#include <pch.h>

#include <aiva/layer1/i_gpu_resource.h>
#include <aiva/layer1/gr_buffer_desc.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	struct Engine;
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
		GrBuffer(Engine const& engine, GrBufferDesc const& desc);

	public:
		~GrBuffer();

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// High-Level Data

	public:
		GrBufferDesc Desc() const;

		GrBuffer& Desc(GrBufferDesc const& desc);

	// ----------------------------------------------------
	// Low-Level Data

	public:
		winrt::com_ptr<ID3D12Resource> const InternalResource();

		aiva::utils::EvAction& OnInternalResourceUpdated();

	private:
		void UpdateInternalResource(GrBufferDesc const& desc);

	private:
		winrt::com_ptr<ID3D12Resource> mInternalResource{};

		aiva::utils::EvAction mOnInternalResourceUpdated{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrBuffer> aiva::layer1::GrBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<GrBuffer>{new GrBuffer{ std::forward<TArgs>(args)... }};
}
