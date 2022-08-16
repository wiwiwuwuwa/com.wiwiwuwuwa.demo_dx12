#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct AGraphicResource;

	using AGraphicResourceType = AGraphicResource;
	using AGraphicResourceTypeShared = std::shared_ptr<AGraphicResourceType>;
	using AGraphicResourceTypeWeak = std::weak_ptr<AGraphicResourceType>;
}
