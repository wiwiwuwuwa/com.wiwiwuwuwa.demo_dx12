#pragma once
#include <pch.h>

#include <aiva/utils/e_cache_flags.h>
#include <aiva/utils/t_ev_action.h>

namespace aiva::utils
{
	template <typename TDirtyFlags = ECacheFlags>
	struct TObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	protected:
		TObjectChangeable();

	public:
		virtual ~TObjectChangeable();

	// ----------------------------------------------------
	// Aliases

	public:
		using FlagType = TDirtyFlags;

		using ThisType = typename TObjectChangeable<typename FlagType>;

		using FuncType = typename TEvAction<typename FlagType>;

	// ----------------------------------------------------
	// Change Event

	public:
		typename FuncType& OnCacheDataChanged();

	protected:
		void BroadcastCacheDataChanged(typename FlagType const dirtyFlags = FlagType::All);

	private:
		typename FuncType mOnCacheDataChanged{};
	};
}

// --------------------------------------------------------

namespace aiva::utils
{
	template <typename TDirtyFlags>
	TObjectChangeable<TDirtyFlags>::TObjectChangeable()
	{

	}

	template <typename TDirtyFlags>
	TObjectChangeable<TDirtyFlags>::~TObjectChangeable()
	{

	}

	template <typename TDirtyFlags>
	typename TObjectChangeable<TDirtyFlags>::FuncType& TObjectChangeable<TDirtyFlags>::OnCacheDataChanged()
	{
		return mOnCacheDataChanged;
	}

	template <typename TDirtyFlags>
	void TObjectChangeable<TDirtyFlags>::BroadcastCacheDataChanged(typename FlagType const dirtyFlags /*= FlagType::All*/)
	{
		OnCacheDataChanged()(dirtyFlags);
	}
}
