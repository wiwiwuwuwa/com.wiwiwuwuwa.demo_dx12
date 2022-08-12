#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct RoJson;

	using RoJsonType = RoJson;
	using RoJsonTypeShared = std::shared_ptr<RoJsonType>;
	using RoJsonTypeWeak = std::weak_ptr<RoJsonType>;
}
