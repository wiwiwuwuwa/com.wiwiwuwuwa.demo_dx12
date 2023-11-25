#pragma once
#include <pch.h>

#include <aiva/utils_ext/t_event_action_readwrite.h>

namespace aiva::utils_ext
{
	template <typename TOwnerType, typename... TArgs>
	using TEventAction = TEventActionReadWrite<TOwnerType, TArgs...>;
}
