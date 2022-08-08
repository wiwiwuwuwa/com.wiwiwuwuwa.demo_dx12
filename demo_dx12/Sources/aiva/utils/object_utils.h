#pragma once
#include <pch.h>

#include <aiva/utils/object_factory.h>

namespace aiva::utils
{
	template <typename TObject, typename... TArgs>
	std::shared_ptr<TObject> NewObject(TArgs&&... args);
}

// --------------------------------------------------------

template <typename TObject, typename... TArgs>
std::shared_ptr<TObject> aiva::utils::NewObject(TArgs&&... args)
{
	return TObject::FactoryType::Create<TObject>(std::forward<TArgs>(args)...);
}
