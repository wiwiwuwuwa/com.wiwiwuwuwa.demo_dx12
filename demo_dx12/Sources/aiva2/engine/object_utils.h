#pragma once
#include <pch.h>

namespace aiva2::engine
{
	template <typename TObjectType, typename... TArgsType>
	auto NewObject(TArgsType&&... args);
}

// --------------------------------------------------------

namespace aiva2::engine
{
	template <typename TObjectType, typename... TArgsType>
	auto NewObject(TArgsType&&... args)
	{
		return std::shared_ptr<TObjectType>{new TObjectType{ std::forward<TArgsType>(args)... }};
	}
}
