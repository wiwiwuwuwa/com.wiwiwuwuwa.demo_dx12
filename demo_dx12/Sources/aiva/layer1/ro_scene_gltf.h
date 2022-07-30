#pragma once
#include <pch.h>

#include <aiva/layer1/i_cpu_resource.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct RoSceneGltf final : public aiva::layer1::ICpuResource
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<aiva::layer1::RoSceneGltf> Create(Args&&... args);

	private:
		RoSceneGltf(aiva::layer1::Engine const& engine);

	public:
		~RoSceneGltf();

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// ICpuResource

	public:
		void DeserealizeFromBinary(std::vector<std::byte> const& binaryData) override;
	};
}

// --------------------------------------------------------

template <typename... Args>
std::shared_ptr<aiva::layer1::RoSceneGltf> aiva::layer1::RoSceneGltf::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer1::RoShaderCompute>{new aiva::layer1::RoShaderCompute{ std::forward<Args>(args)... }};
}
