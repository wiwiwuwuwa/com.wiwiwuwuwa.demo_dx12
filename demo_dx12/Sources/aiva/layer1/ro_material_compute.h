#pragma once
#include <pch.h>

#include <aiva/layer1/i_resource_object.h>
#include <aiva/utils/changes_counter.h>

namespace aiva::layer1
{
	struct Engine;
	struct RoShaderCompute;
}

namespace aiva::layer1
{
	struct RoMaterialCompute final : public aiva::layer1::IResourceObject
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<aiva::layer1::RoMaterialCompute> Create(Args&&... args);

	private:
		RoMaterialCompute(aiva::layer1::Engine const& engine);

	public:
		~RoMaterialCompute();

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// IResourceObject

	public:
		void Deserealize(std::vector<std::byte> const& binaryData) override;

	// ----------------------------------------------------
	// Changes Detection

	public:
		RoMaterialCompute& BeginChanges();

		RoMaterialCompute& EndChanges();

	private:
		aiva::utils::ChangesCounter mChangesCounter{};

	// ----------------------------------------------------
	// High-Level Data

	public:
		std::shared_ptr<aiva::layer1::RoShaderCompute> const& Shader() const;

		aiva::layer1::RoMaterialCompute& Shader(std::shared_ptr<aiva::layer1::RoShaderCompute> const& shader);

	private:
		std::shared_ptr<aiva::layer1::RoShaderCompute> mShader{};

	// ----------------------------------------------------
	// Low-Level Data

	public:
		winrt::com_ptr<ID3D12RootSignature> const& RootSignature() const;

	private:
		void InitializeLowLevelData();

		void TerminateLowLevelData();

	private:
		void RefreshLowLevelData();

		void RefreshRootSignature();

		void RefreshComputePipelineState();

	private:
		winrt::com_ptr<ID3D12RootSignature> mRootSignature{};

		winrt::com_ptr<ID3D12PipelineState> mPipelineState{};
	};
}

// --------------------------------------------------------

template <typename... Args>
std::shared_ptr<aiva::layer1::RoMaterialCompute> aiva::layer1::RoMaterialCompute::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer1::RoMaterialCompute>{new aiva::layer1::RoMaterialCompute{ std::forward<Args>(args)... }};
}
