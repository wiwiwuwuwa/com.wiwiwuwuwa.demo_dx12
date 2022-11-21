#pragma once
#include <pch.h>

#include <aiva/utils_ext/t_dirty_flags_eager_readonly.h>
#include <aiva/utils_ext/m_object_field_val_event_action.h>

namespace aiva::utils_ext
{
	template <typename TOwnerType, typename TFlagsType = EDirtyFlags>
	struct TDirtyFlagsEagerReadWrite : public TDirtyFlagsEagerReadOnly<TFlagsType>
	{
	// ----------------------------------------------------
	// Main

	public:
		using OwnerType = TOwnerType;

		using FlagsType = TFlagsType;

		using BaseType = TDirtyFlagsEagerReadOnly<FlagsType>;

		using ThisType = TDirtyFlagsEagerReadWrite<OwnerType, FlagsType>;

	public:
		friend OwnerType;

	protected:
		TDirtyFlagsEagerReadWrite();

	public:
		~TDirtyFlagsEagerReadWrite() override;

	// ----------------------------------------------------
	// Generic Fields

	protected:
		void Dirty(FlagsType const flags = FlagsType::Everything) override;

	// ----------------------------------------------------
	// Writer API

	protected:
		M_OBJECT_FIELD_VAL_EVENT_ACTION_B2(protected, OnBeforeDirtied, FlagsType);
	};
}

// --------------------------------------------------------

namespace aiva::utils_ext
{
	template <typename TOwnerType, typename TFlagsType>
	TDirtyFlagsEagerReadWrite<TOwnerType, TFlagsType>::TDirtyFlagsEagerReadWrite()
	{

	}

	template <typename TOwnerType, typename TFlagsType>
	TDirtyFlagsEagerReadWrite<TOwnerType, TFlagsType>::~TDirtyFlagsEagerReadWrite()
	{

	}

	template <typename TOwnerType, typename TFlagsType>
	void TDirtyFlagsEagerReadWrite<TOwnerType, TFlagsType>::Dirty(FlagsType const flags /*= FlagsType::Everything*/)
	{
		OnBeforeDirtied().Broadcast(flags);
		BaseType::Dirty(flags);
	}
}
