#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct ResourceViewHeap;

	using ResourceViewHeapType = ResourceViewHeap;
	using ResourceViewHeapTypeShared = std::shared_ptr<ResourceViewHeapType>;
	using ResourceViewHeapTypeWeak = std::weak_ptr<ResourceViewHeapType>;
}
