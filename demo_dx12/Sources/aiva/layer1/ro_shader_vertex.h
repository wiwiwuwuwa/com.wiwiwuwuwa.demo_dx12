#pragma once
#include <pch.h>

#include <aiva/layer1/i_cpu_resource.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct RoShaderVertex final : public aiva::layer1::ICpuResource
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<aiva::layer1::RoShaderVertex> Create(Args&&... args);

	private:
		RoShaderVertex(aiva::layer1::Engine const& engine);

	public:
		~RoShaderVertex();

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// ICpuResource

	public:
		void DeserealizeFromBinary(std::vector<std::byte> const& binaryData) override;

	// ----------------------------------------------------
	// Low-Level Data

	public:
		winrt::com_ptr<ID3DBlob> const& Bytecode() const;

	private:
		winrt::com_ptr<ID3DBlob> mBytecode{};
	};
}

// --------------------------------------------------------

template <typename... Args>
std::shared_ptr<aiva::layer1::RoShaderVertex> aiva::layer1::RoShaderVertex::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer1::RoShaderVertex>{new aiva::layer1::RoShaderVertex{ std::forward<Args>(args)... }};
}
