#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct MaterialCachingSystem;

	using MaterialCachingSystemType = MaterialCachingSystem;
	using MaterialCachingSystemTypeShared = std::shared_ptr<MaterialCachingSystemType>;
	using MaterialCachingSystemTypeWeak = std::weak_ptr<MaterialCachingSystemType>;
}
