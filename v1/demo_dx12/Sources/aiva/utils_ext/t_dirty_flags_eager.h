#pragma once
#include <pch.h>

#include <aiva/utils_ext/t_dirty_flags_eager_readwrite.h>

namespace aiva::utils_ext
{
	template <typename TOwnerType, typename TFlagsType = EDirtyFlags>
	using TDirtyFlagsEager = TDirtyFlagsEagerReadWrite<TOwnerType, TFlagsType>;
}
