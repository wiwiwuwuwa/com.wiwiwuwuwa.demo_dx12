#pragma once
#include <pch.h>

#include <aiva/utils_ext/t_dirty_flags_eager_base.h>
#include <aiva/utils_ext/m_object_field_val_event_action.h>

namespace aiva::utils_ext
{
	template <typename TFlagsType = EDirtyFlags>
	struct TDirtyFlagsEagerReadOnly : public TDirtyFlagsEagerBase<TFlagsType>
	{
	// ----------------------------------------------------
	// Main

	public:
		using FlagsType = TFlagsType;

		using BaseType = TDirtyFlagsEagerBase<FlagsType>;

		using ThisType = TDirtyFlagsEagerReadOnly<FlagsType>;

	protected:
		TDirtyFlagsEagerReadOnly();

	public:
		~TDirtyFlagsEagerReadOnly() override;

	// ----------------------------------------------------
	// Generic Fields

	protected:
		void Dirty(FlagsType const flags = FlagsType::Everything) override;

	// ----------------------------------------------------
	// Reader API

	public:
		M_OBJECT_FIELD_VAL_EVENT_ACTION_B2(public, OnAfterDirtied, FlagsType);
	};
}

// --------------------------------------------------------

namespace aiva::utils_ext
{
	template <typename TFlagsType>
	TDirtyFlagsEagerReadOnly<TFlagsType>::TDirtyFlagsEagerReadOnly()
	{

	}

	template <typename TFlagsType>
	TDirtyFlagsEagerReadOnly<TFlagsType>::~TDirtyFlagsEagerReadOnly()
	{

	}

	template <typename TFlagsType>
	void TDirtyFlagsEagerReadOnly<TFlagsType>::Dirty(FlagsType const flags /*= FlagsType::Everything*/)
	{
		OnAfterDirtied().Broadcast(flags);
		BaseType::Dirty(flags);
	}
}
