#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GraphicResourceFactory final
	{
	// ----------------------------------------------------
	// Main

	private:
		GraphicResourceFactory() = delete;

	// ----------------------------------------------------
	// Factory

	public:
		template <typename TGraphicResource, typename... TArgs>
		static std::shared_ptr<TGraphicResource> Create(TArgs&&... args);
	};
}

// --------------------------------------------------------

template <typename TGraphicResource, typename... TArgs>
std::shared_ptr<TGraphicResource> aiva::layer1::GraphicResourceFactory::Create(TArgs&&... args)
{
	return std::shared_ptr<TGraphicResource>{new TGraphicResource{ std::forward<TArgs>(args)... }};
}
