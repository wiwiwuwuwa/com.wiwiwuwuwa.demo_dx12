#pragma once
#include <pch.h>

namespace aiva::layer2
{
	struct ScMeshRenderer;

	using ScMeshRendererType = ScMeshRenderer;
	using ScMeshRendererTypeShared = std::shared_ptr<ScMeshRendererType>;
	using ScMeshRendererTypeWeak = std::weak_ptr<ScMeshRendererType>;
}
