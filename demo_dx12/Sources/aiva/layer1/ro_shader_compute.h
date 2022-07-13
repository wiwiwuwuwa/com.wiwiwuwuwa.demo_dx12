#pragma once
#include <pch.h>

#include <aiva/layer1/i_resource_object.h>

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
		RoShaderCompute();

		RoShaderCompute(aiva::layer1::Engine const& engine, std::vector<std::byte> const& binaryData);

	public:
		~RoShaderCompute();
	};
}

// --------------------------------------------------------

template <typename... Args>
std::shared_ptr<aiva::layer1::RoShaderCompute> aiva::layer1::RoShaderCompute::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer1::RoShaderCompute>{new aiva::layer1::RoShaderCompute{ std::forward<Args>(args)... }};
}
