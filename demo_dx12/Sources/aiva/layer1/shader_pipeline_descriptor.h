#pragma once
#include <pch.h>

#include <aiva/layer1/e_gpu_comparison_func.h>
#include <aiva/layer1/e_gpu_cull_mode.h>
#include <aiva/layer1/e_gpu_fill_mode.h>
#include <aiva/layer1/e_gpu_resource_buffer_format.h>
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
	struct ShaderPipelineDescriptor final : private boost::noncopyable, public std::enable_shared_from_this<ShaderPipelineDescriptor>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<ShaderPipelineDescriptor> Create(TArgs&&... args);

	private:
		ShaderPipelineDescriptor(Engine const& engine);

	public:
		~ShaderPipelineDescriptor();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Cache Refresh

	public:
		enum class EDirtyFlags
		{
			None = 0,
			All = 1,
		};

		using CacheUpdaterType = aiva::utils::TCacheUpdater<EDirtyFlags, ShaderPipelineDescriptor>;

	public:
		CacheUpdaterType& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};

	// ----------------------------------------------------
	// Fill Mode

	public:
		EGpuFillMode FillMode() const;

		ShaderPipelineDescriptor& FillMode(EGpuFillMode const fillMode);

	private:
		EGpuFillMode mFillMode{ EGpuFillMode::Solid };

	// ----------------------------------------------------
	// Cull Mode

	public:
		EGpuCullMode CullMode() const;

		ShaderPipelineDescriptor& CullMode(EGpuCullMode const cullMode);

	private:
		EGpuCullMode mCullMode{ EGpuCullMode::Back };

	// ----------------------------------------------------
	// Depth Test

	public:
		bool DepthTest() const;

		ShaderPipelineDescriptor& DepthTest(bool const depthTest);

	private:
		bool mDepthTest{ true };

	// ----------------------------------------------------
	// Depth Write

	public:
		bool DepthWrite() const;

		ShaderPipelineDescriptor& DepthWrite(bool const depthWrite);

	private:
		bool mDepthWrite{ true };

	// ----------------------------------------------------
	// Depth Func

	public:
		EGpuComparisonFunc DepthFunc() const;

		ShaderPipelineDescriptor& DepthFunc(EGpuComparisonFunc const depthFunc);

	private:
		EGpuComparisonFunc mDepthFunc{ EGpuComparisonFunc::LessEqual };

	// ----------------------------------------------------
	// Render Targets

	public:
		std::vector<EGpuResourceBufferFormat> RenderTargets() const;

		ShaderPipelineDescriptor& RenderTargets(std::vector<EGpuResourceBufferFormat> const& renderTargets);

	private:
		std::vector<EGpuResourceBufferFormat> mRenderTargets = { EGpuResourceBufferFormat::R32G32B32A32_FLOAT };

	// ----------------------------------------------------
	// Depth Target

	public:
		EGpuResourceBufferFormat DepthTarget() const;

		ShaderPipelineDescriptor& DepthTarget(EGpuResourceBufferFormat const depthTarget);

	private:
		EGpuResourceBufferFormat mDepthTarget{ EGpuResourceBufferFormat::D32_FLOAT };
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::ShaderPipelineDescriptor> aiva::layer1::ShaderPipelineDescriptor::Create(TArgs&&... args)
{
	return std::shared_ptr<ShaderPipelineDescriptor>{new ShaderPipelineDescriptor{ std::forward<TArgs>(args)... }};
}
