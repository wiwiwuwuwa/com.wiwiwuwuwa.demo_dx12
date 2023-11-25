#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct ResourceViewTable;

	using ResourceViewTableType = ResourceViewTable;
	using ResourceViewTableTypeShared = std::shared_ptr<ResourceViewTableType>;
	using ResourceViewTableTypeWeak = std::weak_ptr<ResourceViewTableType>;
}
