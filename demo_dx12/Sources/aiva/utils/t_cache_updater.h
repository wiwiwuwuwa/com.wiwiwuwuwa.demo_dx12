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
	// Types

	public:
		using FlagType = TDirtyFlags;

		using ActionType = TEvAction<FlagType>;

	// ----------------------------------------------------
	// Constructors

	public:
		TCacheUpdaterBase(FlagType const dirtyFlags = FlagType::All);

		virtual ~TCacheUpdaterBase();

	// ----------------------------------------------------
	// User API

	public:
		void FlushChanges(FlagType const dirtyFlags = FlagType::All);

		ActionType& OnMarkAsChanged();

		ActionType& OnFlushExecuted();

	// ----------------------------------------------------
	// Owner API

	protected:
		void MarkAsChanged(FlagType const dirtyFlags = FlagType::All);

		void ClearChanges(FlagType const dirtyFlags = FlagType::All);

		ActionType& FlushExecutors();

	// ----------------------------------------------------
	// Self API

	private:
		FlagType mDirtyFlags{};

		bool mIsFlushingCache{};

		ActionType mOnMarkAsChanged{};

		ActionType mOnFlushExecuted{};

		ActionType mFlushExecutors{};
	};

	template <typename TOwnerType, typename TDirtyFlags = ECacheFlags>
	struct TCacheUpdater final : public TCacheUpdaterBase<TDirtyFlags>
	{
	// ----------------------------------------------------
	// Types

	public:
		using FlagType = typename TCacheUpdaterBase::FlagType;

		using ActionType = typename TCacheUpdaterBase::ActionType;

	// ----------------------------------------------------
	// Constructors

	private:
		friend TOwnerType;

	public:
		TCacheUpdater(FlagType const dirtyFlags = FlagType::All);

		~TCacheUpdater() override;
	};
}

// --------------------------------------------------------

template <typename TDirtyFlags>
aiva::utils::TCacheUpdaterBase<TDirtyFlags>::TCacheUpdaterBase(FlagType const dirtyFlags /*= FlagType::All*/) : mDirtyFlags{ dirtyFlags }
{

}

template <typename TDirtyFlags>
aiva::utils::TCacheUpdaterBase<TDirtyFlags>::~TCacheUpdaterBase()
{

}

template <typename TDirtyFlags>
void aiva::utils::TCacheUpdaterBase<TDirtyFlags>::FlushChanges(FlagType const dirtyFlags /*= FlagType::All*/)
{
	auto const deltaFlags = EnumUtils::And(mDirtyFlags, dirtyFlags);
	if (deltaFlags == FlagType{})
	{
		return;
	}

	mDirtyFlags = EnumUtils::Clear(mDirtyFlags, deltaFlags);

	mIsFlushingCache = true;
	FlushExecutors()(deltaFlags);
	mIsFlushingCache = false;

	OnFlushExecuted()(deltaFlags);
}

template <typename TDirtyFlags>
typename aiva::utils::TCacheUpdaterBase<TDirtyFlags>::ActionType& aiva::utils::TCacheUpdaterBase<TDirtyFlags>::OnMarkAsChanged()
{
	return mOnMarkAsChanged;
}

template <typename TDirtyFlags>
typename aiva::utils::TCacheUpdaterBase<TDirtyFlags>::ActionType& aiva::utils::TCacheUpdaterBase<TDirtyFlags>::OnFlushExecuted()
{
	return mOnFlushExecuted;
}

template <typename TDirtyFlags>
void aiva::utils::TCacheUpdaterBase<TDirtyFlags>::MarkAsChanged(FlagType const dirtyFlags /*= FlagType::All*/)
{
	if (mIsFlushingCache)
	{
		return;
	}

	auto const deltaFlags = EnumUtils::New(mDirtyFlags, dirtyFlags);
	if (deltaFlags == TDirtyFlags{})
	{
		return;
	}

	mDirtyFlags = EnumUtils::Or(mDirtyFlags, deltaFlags);
	OnMarkAsChanged()(deltaFlags);
}

template <typename TDirtyFlags>
void aiva::utils::TCacheUpdaterBase<TDirtyFlags>::ClearChanges(FlagType const dirtyFlags /*= FlagType::All*/)
{
	mDirtyFlags = EnumUtils::Clear(mDirtyFlags, dirtyFlags);
}

template <typename TDirtyFlags>
typename aiva::utils::TCacheUpdaterBase<TDirtyFlags>::ActionType& aiva::utils::TCacheUpdaterBase<TDirtyFlags>::FlushExecutors()
{
	return mFlushExecutors;
}

template <typename TOwnerType, typename TDirtyFlags>
aiva::utils::TCacheUpdater<TOwnerType, TDirtyFlags>::TCacheUpdater(FlagType const dirtyFlags /*= FlagType::All*/) : TCacheUpdaterBase{ dirtyFlags }
{

}

template <typename TOwnerType, typename TDirtyFlags>
aiva::utils::TCacheUpdater<TOwnerType, TDirtyFlags>::~TCacheUpdater()
{

}
