#pragma once
#include <pch.h>

namespace aiva2::engine
{
	template <typename TObjectType, typename... TArgsType>
	auto NewObject(TArgsType&&... args);
}

// --------------------------------------------------------

#include <aiva2/engine/asserts.h>

namespace aiva2::engine
{
	template <typename TObjectType, typename... TArgsType>
	auto NewObject(TArgsType&&... args)
	{
		auto const obj = new TObjectType{ std::forward<TArgsType>(args)... };
		Asserts::IsTrue(obj, "Obj is not valid");

		obj->Init();

		auto const dst = [](TObjectType* const tmp)
		{
			Asserts::IsTrue(tmp, "Tmp is not valid");
			tmp->Shut();
			delete tmp;
		};

		return std::shared_ptr<TObjectType>{ obj, dst };
	}
}
