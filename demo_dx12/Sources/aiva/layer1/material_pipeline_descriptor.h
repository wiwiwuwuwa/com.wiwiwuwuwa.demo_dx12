#pragma once
#include <pch.h>

#include <aiva/layer1/e_comparison_func.h>
#include <aiva/layer1/e_cull_mode.h>
#include <aiva/layer1/e_fill_mode.h>
#include <aiva/layer1/e_resource_buffer_format.h>
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
	struct MaterialPipelineDescriptor final : private boost::noncopyable, public std::enable_shared_from_this<MaterialPipelineDescriptor>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<MaterialPipelineDescriptor> Create(TArgs&&... args);

	private:
		MaterialPipelineDescriptor(Engine const& engine);

	public:
		~MaterialPipelineDescriptor();

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

		using CacheUpdaterType = aiva::utils::TCacheUpdater<MaterialPipelineDescriptor, EDirtyFlags>;

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
		EFillMode FillMode() const;

		MaterialPipelineDescriptor& FillMode(EFillMode const fillMode);

	private:
		EFillMode mFillMode{ EFillMode::Solid };

	// ----------------------------------------------------
	// Cull Mode

	public:
		ECullMode CullMode() const;

		MaterialPipelineDescriptor& CullMode(ECullMode const cullMode);

	private:
		ECullMode mCullMode{ ECullMode::Back };

	// ----------------------------------------------------
	// Depth Test

	public:
		bool DepthTest() const;

		MaterialPipelineDescriptor& DepthTest(bool const depthTest);

	private:
		bool mDepthTest{ true };

	// ----------------------------------------------------
	// Depth Write

	public:
		bool DepthWrite() const;

		MaterialPipelineDescriptor& DepthWrite(bool const depthWrite);

	private:
		bool mDepthWrite{ true };

	// ----------------------------------------------------
	// Depth Func

	public:
		EComparisonFunc DepthFunc() const;

		MaterialPipelineDescriptor& DepthFunc(EComparisonFunc const depthFunc);

	private:
		EComparisonFunc mDepthFunc{ EComparisonFunc::LessEqual };

	// ----------------------------------------------------
	// Render Targets

	public:
		std::vector<EResourceBufferFormat> RenderTargets() const;

		MaterialPipelineDescriptor& RenderTargets(std::vector<EResourceBufferFormat> const& renderTargets);

	private:
		std::vector<EResourceBufferFormat> mRenderTargets = { EResourceBufferFormat::R32G32B32A32_FLOAT };

	// ----------------------------------------------------
	// Depth Target

	public:
		EResourceBufferFormat DepthTarget() const;

		MaterialPipelineDescriptor& DepthTarget(EResourceBufferFormat const depthTarget);

	private:
		EResourceBufferFormat mDepthTarget{ EResourceBufferFormat::D32_FLOAT };

	// ----------------------------------------------------
	// Copying

	public:
		void CopyPropertiesFrom(MaterialPipelineDescriptor const& source);
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::MaterialPipelineDescriptor> aiva::layer1::MaterialPipelineDescriptor::Create(TArgs&&... args)
{
	return std::shared_ptr<MaterialPipelineDescriptor>{new MaterialPipelineDescriptor{ std::forward<TArgs>(args)... }};
}
