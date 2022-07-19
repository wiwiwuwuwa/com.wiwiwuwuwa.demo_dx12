#pragma once
#include <pch.h>

#include <aiva/layer1/i_gpu_resource.h>

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
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrTextureBuffer2D> aiva::layer1::GrTextureBuffer2D::Create(TArgs&&... args)
{
	return std::shared_ptr<aiva::layer1::GrTextureBuffer2D>{new aiva::layer1::GrTextureBuffer2D{ std::forward<TArgs>(args)... }};
}
