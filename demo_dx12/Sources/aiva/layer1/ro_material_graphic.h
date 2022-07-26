#pragma once
#include <pch.h>

#include <aiva/layer1/i_cpu_resource.h>
#include <aiva/utils/ev_action.h>
#include <aiva/utils/t_cache_refresh.h>

namespace aiva::layer1
{
	struct Engine;
	struct RoShaderFragment;
	struct RoShaderVertex;
	struct ShaderPipelineDescriptor;
	struct ShaderResourceDescriptor;
}

namespace aiva::layer1
{
	struct RoMaterialGraphic final : private boost::noncopyable, public std::enable_shared_from_this<RoMaterialGraphic>, public ICpuResource
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<RoMaterialGraphic> Create(Args&&... args);

	private:
		RoMaterialGraphic(Engine const& engine);

	public:
		~RoMaterialGraphic();

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

	public:
		aiva::utils::TCacheRefresh<EDirtyFlags>& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<aiva::utils::TCacheRefresh<EDirtyFlags>> mCacheUpdater{};

	// ----------------------------------------------------
	// Vertex Shader

	public:
		std::shared_ptr<RoShaderVertex> VertexShader() const;

		RoMaterialGraphic& VertexShader(std::shared_ptr<RoShaderVertex> const& vertexShader);

	private:
		std::shared_ptr<RoShaderVertex> mVertexShader{};

	// ----------------------------------------------------
	// Fragment Shader

	public:
		std::shared_ptr<RoShaderFragment> FragmentShader() const;

		RoMaterialGraphic& FragmentShader(std::shared_ptr<RoShaderFragment> const& fragmentShader);

	private:
		std::shared_ptr<RoShaderFragment> mFragmentShader{};

	// ----------------------------------------------------
	// Pipeline Descriptor

	public:
		ShaderPipelineDescriptor& PipelineDescriptor() const;

	private:
		void InitializePipelineDescriptor();

		void TerminatePipelineDescriptor();

	private:
		void OnPipelineDescriptorUpdated();

	private:
		std::shared_ptr<ShaderPipelineDescriptor> mPipelineDescriptor{};

	// ----------------------------------------------------
	// Resource Descriptor

	public:
		ShaderResourceDescriptor& ResourceDescriptor() const;

	private:
		void InitializeResourceDescriptor();

		void TerminateResourceDescriptor();

	private:
		void OnResourceDescriptorUpdated();

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

	// ----------------------------------------------------
	// Internal Resources Events

	public:
		aiva::utils::EvAction& OnInternalResourcesUpdated();

	private:
		aiva::utils::EvAction mOnInternalResourcesUpdated{};
	};
}

// --------------------------------------------------------

template <typename... Args>
std::shared_ptr<aiva::layer1::RoMaterialGraphic> aiva::layer1::RoMaterialGraphic::Create(Args&&... args)
{
	return std::shared_ptr<RoMaterialGraphic>{new RoMaterialGraphic{ std::forward<Args>(args)... }};
}
