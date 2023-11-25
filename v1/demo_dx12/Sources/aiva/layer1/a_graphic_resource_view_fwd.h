#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct AGraphicResourceView;

	using AGraphicResourceViewType = AGraphicResourceView;
	using AGraphicResourceViewTypeShared = std::shared_ptr<AGraphicResourceViewType>;
	using AGraphicResourceViewTypeWeak = std::weak_ptr<AGraphicResourceViewType>;
}
