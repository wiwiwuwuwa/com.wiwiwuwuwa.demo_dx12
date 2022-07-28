#pragma once
#include <pch.h>

#include <aiva/utils/asserts.h>
#include <aiva/utils/e_cache_flags.h>
#include <aiva/utils/enum_utils.h>
#include <aiva/utils/t_ev_action.h>

namespace aiva::utils
{
	template <typename TDirtyFlags = ECacheFlags>
	struct TCacheUpdaterBase : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Constructors

	public:
		TCacheUpdaterBase(TDirtyFlags const dirtyFlags = TDirtyFlags::All);

	// ----------------------------------------------------
	// User API

	public:
		void FlushChanges();

		TEvAction<TDirtyFlags>& OnMarkAsChanged();

		TEvAction<TDirtyFlags>& OnFlushExecuted();

	// ----------------------------------------------------
	// Owner API

	protected:
		void MarkAsChanged(TDirtyFlags const dirtyFlags = TDirtyFlags::All);

		TEvAction<TDirtyFlags>& FlushExecutors();

	// ----------------------------------------------------
	// Self API

	private:
		TDirtyFlags mDirtyFlags{};

		TEvAction<TDirtyFlags> mOnMarkAsChanged{};

		TEvAction<TDirtyFlags> mOnFlushExecuted{};

		TEvAction<TDirtyFlags> mFlushExecutors{};
	};

	template <typename TOwnerType, typename TDirtyFlags = ECacheFlags>
	struct TCacheUpdater final : public TCacheUpdaterBase<TDirtyFlags>
	{
		friend TOwnerType;
	};
}

// --------------------------------------------------------

template <typename TDirtyFlags>
aiva::utils::TCacheUpdaterBase<TDirtyFlags>::TCacheUpdaterBase(TDirtyFlags const dirtyFlags /*= TDirtyFlags::All*/) : mDirtyFlags{ dirtyFlags }
{

}

template <typename TDirtyFlags>
void aiva::utils::TCacheUpdaterBase<TDirtyFlags>::FlushChanges()
{
	auto const dirtyFlags = mDirtyFlags;
	if (dirtyFlags == TDirtyFlags{})
	{
		return;
	}

	mDirtyFlags = {};
	FlushExecutors()(dirtyFlags);
	OnFlushExecuted()(dirtyFlags);
}

template <typename TDirtyFlags>
aiva::utils::TEvAction<TDirtyFlags>& aiva::utils::TCacheUpdaterBase<TDirtyFlags>::OnMarkAsChanged()
{
	return mOnMarkAsChanged;
}

template <typename TDirtyFlags>
aiva::utils::TEvAction<TDirtyFlags>& aiva::utils::TCacheUpdaterBase<TDirtyFlags>::OnFlushExecuted()
{
	return mOnFlushExecuted;
}

template <typename TDirtyFlags>
void aiva::utils::TCacheUpdaterBase<TDirtyFlags>::MarkAsChanged(TDirtyFlags const dirtyFlags /*= TDirtyFlags::All*/)
{
	mDirtyFlags = EnumUtils::Or(mDirtyFlags, dirtyFlags);
	OnMarkAsChanged()(dirtyFlags);
}

template <typename TDirtyFlags>
aiva::utils::TEvAction<TDirtyFlags>& aiva::utils::TCacheUpdaterBase<TDirtyFlags>::FlushExecutors()
{
	return mFlushExecutors;
}
