#pragma once
#include <pch.h>

namespace aiva::utils
{
	template <typename... TArgs>
	using TEvAction = boost::signals2::signal<void(TArgs...)>;
}
