#pragma once
#include <pch.h>

#include <aiva/layer1/i_cpu_resource.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	struct Engine;
	struct RoShaderCompute;
	struct ShaderResourceDescriptor;
}

namespace aiva::utils
{
	template <typename, typename>
	struct TCacheUpdater;
}

namespace aiva::layer1
{
	struct RoMaterialCompute final : private boost::noncopyable, public std::enable_shared_from_this<RoMaterialCompute>, public ICpuResource
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<RoMaterialCompute> Create(Args&&... args);

	private:
		RoMaterialCompute(Engine const& engine);

	public:
		~RoMaterialCompute();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// ICpuResource

	public:
		void DeserealizeFromBinary(std::vector<std::byte> const& binaryData) override;

	// ----------------------------------------------------
	// Cache Refresh

	public:
		enum class EDirtyFlags
		{
			None = 0,
			All = 1,
		};

		using CacheUpdaterType = aiva::utils::TCacheUpdater<EDirtyFlags, RoMaterialCompute>;

	public:
		CacheUpdaterType& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};

	// ----------------------------------------------------
	// Resource Shader

	public:
		std::shared_ptr<RoShaderCompute> Shader() const;

		RoMaterialCompute& Shader(std::shared_ptr<RoShaderCompute> const& shader);

	private:
		std::shared_ptr<RoShaderCompute> mShader{};

	// ----------------------------------------------------
	// Resource Descriptor

	public:
		ShaderResourceDescriptor& ResourceDescriptor() const;

	private:
		void InitializeResourceDescriptor();

		void TerminateResourceDescriptor();

	private:
		void OnResourceDescriptorMarkedAsChanged();

	private:
		std::shared_ptr<ShaderResourceDescriptor> mResourceDescriptor{};

	// ----------------------------------------------------
	// Internal Resources Data

	public:
		winrt::com_ptr<ID3D12PipelineState> const& InternalPipelineState() const;

	private:
		void InitializeInternalResources();

		void TerminateInternalResources();

	private:
		void RefreshInternalResources();

		void RefreshInternalPipelineState();

	private:
		winrt::com_ptr<ID3D12PipelineState> mInternalPipelineState{};
	};
}

// --------------------------------------------------------

template <typename... Args>
std::shared_ptr<aiva::layer1::RoMaterialCompute> aiva::layer1::RoMaterialCompute::Create(Args&&... args)
{
	return std::shared_ptr<RoMaterialCompute>{new RoMaterialCompute{ std::forward<Args>(args)... }};
}
