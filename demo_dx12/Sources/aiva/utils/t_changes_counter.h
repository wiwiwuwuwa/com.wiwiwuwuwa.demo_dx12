#pragma once
#include <pch.h>

#include <aiva/utils/asserts.h>
#include <aiva/utils/enum_utils.h>
#include <aiva/utils/t_ev_action.h>

namespace aiva::utils
{
	template <typename TDirtyFlags, typename TEnable = void>
	struct TChangesCounter;

	template <typename TDirtyFlags>
	struct TChangesCounter<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>> final : private boost::noncopyable
	{
	public:
		aiva::utils::TEvAction<TDirtyFlags>& OnChangesFinished();

		void IncrementChanges(TDirtyFlags const& dirtyFlags);

		void DecrementChanges(TDirtyFlags const& dirtyFlags);

	private:
		aiva::utils::TEvAction<TDirtyFlags> mOnChangesFinished{};

		std::int32_t mChangesCounter{};

		TDirtyFlags mDirtyFlags{};
	};
}

// --------------------------------------------------------

template <typename TDirtyFlags>
aiva::utils::TEvAction<TDirtyFlags>& aiva::utils::TChangesCounter<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>>::OnChangesFinished()
{
	return mOnChangesFinished;
}

template <typename TDirtyFlags>
void aiva::utils::TChangesCounter<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>>::IncrementChanges(TDirtyFlags const& dirtyFlags)
{
	mDirtyFlags = aiva::utils::EnumUtils::Or(mDirtyFlags, dirtyFlags);
	mChangesCounter++;
}

template <typename TDirtyFlags>
void aiva::utils::TChangesCounter<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>>::DecrementChanges(TDirtyFlags const& dirtyFlags)
{
	mDirtyFlags = aiva::utils::EnumUtils::Or(mDirtyFlags, dirtyFlags);
	mChangesCounter--;

	aiva::utils::Asserts::CheckBool(mChangesCounter >= 0);
	if (mChangesCounter > 0)
	{
		return;
	}

	OnChangesFinished()(mDirtyFlags);

	mDirtyFlags = {};
	mChangesCounter = {};
}
