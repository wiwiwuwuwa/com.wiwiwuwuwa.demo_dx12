#pragma once
#include <pch.h>

#include <aiva/layer1/i_cpu_resource.h>
#include <aiva/layer1/i_object_engineable.h>
#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_cacheable.h>
#include <aiva/utils/m_object_body.h>

namespace aiva::layer1
{
	struct MaterialPipelineDescriptor;
	struct RoShaderFragment;
	struct RoShaderVertex;
	struct MaterialResourceDescriptor;
}

namespace aiva::layer1
{
	struct RoMaterialGraphic final : public aiva::utils::AObject, public aiva::utils::IObjectCacheable, public aiva::layer1::IObjectEngineable, public aiva::layer1::ICpuResource
	{
	// ----------------------------------------------------
	// Body

	public:
		M_OBJECT_BODY();

	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<RoMaterialGraphic> Create(Args&&... args);

	private:
		RoMaterialGraphic(EngineType const& engine);

	public:
		~RoMaterialGraphic() override;

	// ----------------------------------------------------
	// ICpuResource

	public:
		void DeserealizeFromBinary(std::vector<std::byte> const& binaryData) override;

	// ----------------------------------------------------
	// Vertex Shader

	public:
		std::shared_ptr<RoShaderVertex> const& VertexShader() const;

		RoMaterialGraphic& VertexShader(std::shared_ptr<RoShaderVertex> const& vertexShader);

	private:
		std::shared_ptr<RoShaderVertex> mVertexShader{};

	// ----------------------------------------------------
	// Fragment Shader

	public:
		std::shared_ptr<RoShaderFragment> const& FragmentShader() const;

		RoMaterialGraphic& FragmentShader(std::shared_ptr<RoShaderFragment> const& fragmentShader);

	private:
		std::shared_ptr<RoShaderFragment> mFragmentShader{};

	// ----------------------------------------------------
	// Pipeline Descriptor

	public:
		MaterialPipelineDescriptor& PipelineDescriptor() const;

	private:
		void InitializePipelineDescriptor();

		void TerminatePipelineDescriptor();

	private:
		void OnPipelineDescriptorMarkedAsChanged();

	private:
		std::shared_ptr<MaterialPipelineDescriptor> mPipelineDescriptor{};

	// ----------------------------------------------------
	// Resource Descriptor

	public:
		MaterialResourceDescriptor& ResourceDescriptor() const;

	private:
		void InitializeResourceDescriptor();

		void TerminateResourceDescriptor();

	private:
		void OnResourceDescriptorMarkedAsChanged();

	private:
		std::shared_ptr<MaterialResourceDescriptor> mResourceDescriptor{};

	// ----------------------------------------------------
	// Internal Resources Data

	public:
		winrt::com_ptr<ID3D12PipelineState> const& InternalPipelineState();

	private:
		void InitializeInternalPipelineState();

		void TerminateInternalPipelineState();

	private:
		void RefreshInternalPipelineState();

	private:
		winrt::com_ptr<ID3D12PipelineState> mInternalPipelineState{};

	// ----------------------------------------------------
	// Resource Barriers

	public:
		std::vector<D3D12_RESOURCE_BARRIER> PrepareBarriers(bool const active);

	// ----------------------------------------------------
	// Copying

	public:
		RoMaterialGraphic& CopyPropertiesFrom(RoMaterialGraphic const& source);

		std::shared_ptr<RoMaterialGraphic> Copy() const;
	};
}

// --------------------------------------------------------

#include <aiva/utils/object_utils.h>

namespace aiva::layer1
{
	template <typename... Args>
	std::shared_ptr<RoMaterialGraphic> RoMaterialGraphic::Create(Args&&... args)
	{
		return aiva::utils::NewObject<RoMaterialGraphic>(std::forward<Args>(args)...);
	}
}
