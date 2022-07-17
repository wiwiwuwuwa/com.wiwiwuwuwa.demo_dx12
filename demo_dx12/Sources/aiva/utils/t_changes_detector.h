#pragma once
#include <pch.h>

#include <aiva/utils/asserts.h>
#include <aiva/utils/enum_utils.h>
#include <aiva/utils/t_ev_action.h>

namespace aiva::utils
{
	template <typename TDirtyFlags, typename TEnable = void>
	struct TChangesDetector;

	template <typename TDirtyFlags>
	struct TChangesDetector<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>> final : private boost::noncopyable
	{
	public:
		void MarkAsChanged(TDirtyFlags const& dirtyFlags);

		void FlushChanges();

		aiva::utils::TEvAction<TDirtyFlags const&>& OnFlushChanges();

	private:
		aiva::utils::TEvAction<TDirtyFlags const&> mOnFlushChanges{};

		TDirtyFlags mDirtyFlags{};
	};
}

// --------------------------------------------------------

template <typename TDirtyFlags>
void aiva::utils::TChangesDetector<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>>::MarkAsChanged(TDirtyFlags const& dirtyFlags)
{
	mDirtyFlags = aiva::utils::EnumUtils::Or(mDirtyFlags, dirtyFlags);
}

template <typename TDirtyFlags>
void aiva::utils::TChangesDetector<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>>::FlushChanges()
{
	OnFlushChanges()(mDirtyFlags);
	mDirtyFlags = {};
}

template <typename TDirtyFlags>
aiva::utils::TEvAction<TDirtyFlags const&>& aiva::utils::TChangesDetector<TDirtyFlags, std::enable_if_t<std::is_enum_v<TDirtyFlags>>>::OnFlushChanges()
{
	return mOnFlushChanges;
}
