#pragma once
#include <pch.h>

#include <aiva/layer1/i_gpu_resource.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct GrTextureBuffer final : private boost::noncopyable, public std::enable_shared_from_this<GrTextureBuffer>, public aiva::layer1::IGpuResource
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<aiva::layer1::GrTextureBuffer> Create(TArgs&&... args);

	private:
		GrTextureBuffer(aiva::layer1::Engine const& engine);

	public:
		~GrTextureBuffer();

	private:
		aiva::layer1::Engine const& mEngine;
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrTextureBuffer> aiva::layer1::GrTextureBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<aiva::layer1::GrTextureBuffer>{new aiva::layer1::GrTextureBuffer{ std::forward<TArgs>(args)... }};
}
