#pragma once
#include <pch.h>

#include <aiva/layer1/i_resource_object.h>
#include <aiva/utils/changes_counter.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct RoShaderCompute final : public aiva::layer1::IResourceObject
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<aiva::layer1::RoShaderCompute> Create(Args&&... args);

	private:
		RoShaderCompute(aiva::layer1::Engine const& engine);

	public:
		~RoShaderCompute();

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// IResourceObject

	public:
		void Deserealize(std::vector<std::byte> const& binaryData) override;

	// ----------------------------------------------------
	// Directx Cache

	public:
		winrt::com_ptr<ID3DBlob> const& CachedBytecode() const;

	private:
		winrt::com_ptr<ID3DBlob> mCachedBytecode{};
	};
}

// --------------------------------------------------------

template <typename... Args>
std::shared_ptr<aiva::layer1::RoShaderCompute> aiva::layer1::RoShaderCompute::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer1::RoShaderCompute>{new aiva::layer1::RoShaderCompute{ std::forward<Args>(args)... }};
}
