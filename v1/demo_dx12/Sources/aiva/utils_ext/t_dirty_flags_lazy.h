#pragma once
#include <pch.h>

#include <aiva/utils/enum_utils.h>
#include <aiva/utils_ext/e_dirty_flags.h>
#include <aiva/utils_ext/m_object_field_val_event_action.h>

namespace aiva::utils_ext
{
	template <typename TOwnerType, typename TFlagsType = EDirtyFlags>
	struct TDirtyFlagsLazy final : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Main

	public:
		using OwnerType = TOwnerType;

		using FlagsType = TFlagsType;

		using ThisType = TDirtyFlagsLazy<OwnerType, FlagsType>;

	public:
		friend OwnerType;

	protected:
		TDirtyFlagsLazy(FlagsType const flags = FlagsType::Nothing);

	public:
		~TDirtyFlagsLazy();

	// ----------------------------------------------------
	// User API

	public:
		M_OBJECT_FIELD_VAL_EVENT_ACTION_B2(public, OnAfterDirtied, FlagsType);

		M_OBJECT_FIELD_VAL_EVENT_ACTION_B2(public, OnAfterFlushed, FlagsType);

	public:
		void Flush(FlagsType const flags = FlagsType::Everything);

	// ----------------------------------------------------
	// Owner API

	protected:
		M_OBJECT_FIELD_VAL_EVENT_ACTION_B2(protected, OnBeforeDirtied, FlagsType);

		M_OBJECT_FIELD_VAL_EVENT_ACTION_B2(protected, OnBeforeFlushed, FlagsType);

	protected:
		void Dirty(FlagsType const flags = FlagsType::Everything);

		void Clear(FlagsType const flags = FlagsType::Everything);

	// ----------------------------------------------------
	// Self API

	private:
		FlagsType mFlags{};
	};
}

// --------------------------------------------------------

namespace aiva::utils_ext
{
	template <typename TOwnerType, typename TFlagsType>
	TDirtyFlagsLazy<TOwnerType, TFlagsType>::TDirtyFlagsLazy(FlagsType const flags /*= FlagsType::Nothing*/) : mFlags{ flags }
	{

	}

	template <typename TOwnerType, typename TFlagsType>
	TDirtyFlagsLazy<TOwnerType, TFlagsType>::~TDirtyFlagsLazy()
	{

	}

	template <typename TOwnerType, typename TFlagsType>
	void TDirtyFlagsLazy<TOwnerType, TFlagsType>::Flush(FlagsType const flags /*= FlagsType::Everything*/)
	{
		while (true)
		{
			auto const delta = aiva::utils::EnumUtils::And(mFlags, flags);
			if (delta == FlagsType{})
			{
				break;
			}

			mFlags = aiva::utils::EnumUtils::Clear(mFlags, delta);

			OnBeforeFlushed().Broadcast(delta);
			OnAfterFlushed().Broadcast(delta);
		}
	}

	template <typename TOwnerType, typename TFlagsType>
	void TDirtyFlagsLazy<TOwnerType, TFlagsType>::Dirty(FlagsType const flags /*= FlagsType::Everything*/)
	{
		while (true)
		{
			auto const delta = aiva::utils::EnumUtils::New(mFlags, flags);
			if (delta == FlagsType{})
			{
				break;
			}

			mFlags = aiva::utils::EnumUtils::Or(mFlags, delta);

			OnBeforeDirtied().Broadcast(delta);
			OnAfterDirtied().Broadcast(delta);
		}
	}

	template <typename TOwnerType, typename TFlagsType>
	void TDirtyFlagsLazy<TOwnerType, TFlagsType>::Clear(FlagsType const flags /*= FlagsType::Everything*/)
	{
		while (true)
		{
			auto const delta = aiva::utils::EnumUtils::And(mFlags, flags);
			if (delta == FlagsType{})
			{
				break;
			}

			mFlags = aiva::utils::EnumUtils::Clear(mFlags, delta);
			OnAfterFlushed().Broadcast(delta);
		}
	}
}
