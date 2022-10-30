#pragma once
#include <pch.h>

namespace aiva::utils_ext
{
	template <typename TObjectType, typename... TArgs>
	std::shared_ptr<TObjectType> NewObject(TArgs&&... args);
}

// --------------------------------------------------------

namespace aiva::utils_ext
{
	template <typename TObjectType, typename... TArgs>
	std::shared_ptr<TObjectType> NewObject(TArgs&&... args)
	{
		return TObjectType::New(std::forward<TArgs>(args)...);
	}
}
