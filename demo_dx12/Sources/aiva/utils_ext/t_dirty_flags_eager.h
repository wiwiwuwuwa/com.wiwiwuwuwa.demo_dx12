#pragma once
#include <pch.h>

#include <aiva/utils/t_ev_action.h>
#include <aiva/utils_ext/e_dirty_flags.h>

namespace aiva::utils_ext
{
	template <typename TOwnerType, typename TFlagsType = EDirtyFlags>
	struct TDirtyFlagsEager final : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Main

	public:
		using OwnerType = TOwnerType;

		using FlagsType = TFlagsType;

		using EventType = aiva::utils::TEvAction<FlagsType>;

	public:
		friend OwnerType;

	protected:
		TDirtyFlagsEager();

	public:
		~TDirtyFlagsEager();

	// ----------------------------------------------------
	// Alien API

	public:
		void AttachListener(typename EventType::slot_type const& listener);

		void RemoveListener(typename EventType::slot_type const& listener);

	// ----------------------------------------------------
	// Owner API

	protected:
		void Broadcast(FlagsType const flags = FlagsType::Everything);

	// ----------------------------------------------------
	// Self API

	private:
		EventType mOnUpdated{};
	};
}

// --------------------------------------------------------

namespace aiva::utils_ext
{
	template <typename TOwnerType, typename TFlagsType>
	TDirtyFlagsEager<TOwnerType, TFlagsType>::TDirtyFlagsEager()
	{

	}

	template <typename TOwnerType, typename TFlagsType>
	TDirtyFlagsEager<TOwnerType, TFlagsType>::~TDirtyFlagsEager()
	{

	}

	template <typename TOwnerType, typename TFlagsType>
	void TDirtyFlagsEager<TOwnerType, TFlagsType>::AttachListener(typename EventType::slot_type const& listener)
	{
		mOnUpdated.connect(listener);
	}

	template <typename TOwnerType, typename TFlagsType>
	void TDirtyFlagsEager<TOwnerType, TFlagsType>::RemoveListener(typename EventType::slot_type const& listener)
	{
		mOnUpdated.disconnect(listener);
	}

	template <typename TOwnerType, typename TFlagsType>
	void TDirtyFlagsEager<TOwnerType, TFlagsType>::Broadcast(FlagsType const flags /*= FlagsType::Everything*/)
	{
		mOnUpdated(flags);
	}
}
