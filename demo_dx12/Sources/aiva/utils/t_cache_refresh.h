#pragma once
#include <pch.h>

#include <aiva/utils/asserts.h>
#include <aiva/utils/enum_utils.h>
#include <aiva/utils/t_ev_action.h>

namespace aiva::utils
{
	template <typename TDirtyFlags, typename TEnable = void>
	struct TCacheRefresh;

	template <typename TDirtyFlags>
	struct TCacheRefresh<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>> final : private boost::noncopyable
	{
	// ------------------------------------------------
	// Constructors

	public:
		TCacheRefresh(TDirtyFlags const dirtyFlags = {});

	// ------------------------------------------------
	// Actions

	public:
		void MarkAsChanged(TDirtyFlags const dirtyFlags);

		void FlushChanges();

	private:
		TDirtyFlags mDirtyFlags{};

	// ----------------------------------------------------
	// Events

	public:
		aiva::utils::TEvAction<TDirtyFlags>& OnFlushRequested();

		aiva::utils::TEvAction<TDirtyFlags>& OnFlushCompleted();

	private:
		aiva::utils::TEvAction<TDirtyFlags> mOnFlushRequested{};

		aiva::utils::TEvAction<TDirtyFlags> mOnFlushCompleted{};
	};
}

// --------------------------------------------------------

template <typename TDirtyFlags>
aiva::utils::TCacheRefresh<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>>::TCacheRefresh(TDirtyFlags const dirtyFlags /*={}*/) : mDirtyFlags{dirtyFlags}
{

}

template <typename TDirtyFlags>
void aiva::utils::TCacheRefresh<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>>::MarkAsChanged(TDirtyFlags const dirtyFlags)
{
	mDirtyFlags = aiva::utils::EnumUtils::Or(mDirtyFlags, dirtyFlags);
}

template <typename TDirtyFlags>
void aiva::utils::TCacheRefresh<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>>::FlushChanges()
{
	TDirtyFlags const dirtyFlags = mDirtyFlags;

	if (dirtyFlags == TDirtyFlags{})
	{
		return;
	}

	OnFlushRequested()(dirtyFlags);
	OnFlushCompleted()(dirtyFlags);

	mDirtyFlags = {};
}

template <typename TDirtyFlags>
aiva::utils::TEvAction<TDirtyFlags>& aiva::utils::TCacheRefresh<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>>::OnFlushRequested()
{
	return mOnFlushRequested;
}

template <typename TDirtyFlags>
aiva::utils::TEvAction<TDirtyFlags>& aiva::utils::TCacheRefresh<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>>::OnFlushCompleted()
{
	return mOnFlushCompleted;
}
