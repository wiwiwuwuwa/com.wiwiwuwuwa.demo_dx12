#pragma once
#include <pch.h>

#include <aiva/utils_ext/e_dirty_flags.h>

namespace aiva::utils_ext
{
	template <typename TFlagsType = EDirtyFlags>
	struct TDirtyFlagsEagerBase : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Main

	public:
		using FlagsType = TFlagsType;

	protected:
		TDirtyFlagsEagerBase();

	public:
		virtual ~TDirtyFlagsEagerBase();

	// ----------------------------------------------------
	// Generic Fields

	protected:
		virtual void Dirty(FlagsType const flags = FlagsType::Everything);
	};
}

// --------------------------------------------------------

namespace aiva::utils_ext
{
	template <typename TFlagsType>
	TDirtyFlagsEagerBase<TFlagsType>::TDirtyFlagsEagerBase()
	{

	}

	template <typename TFlagsType>
	TDirtyFlagsEagerBase<TFlagsType>::~TDirtyFlagsEagerBase()
	{

	}

	template <typename TFlagsType>
	void TDirtyFlagsEagerBase<TFlagsType>::Dirty(FlagsType const /*= FlagsType::Everything*/)
	{

	}
}
