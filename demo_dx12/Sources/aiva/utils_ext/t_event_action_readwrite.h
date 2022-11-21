#pragma once
#include <pch.h>

#include <aiva/utils_ext/t_event_action_readonly.h>

namespace aiva::utils_ext
{
	template <typename TOwnerType, typename... TArgs>
	struct TEventActionReadWrite : public TEventActionReadOnly<TArgs...>
	{
	// ----------------------------------------------------
	// Main

	public:
		using OwnerType = TOwnerType;

	public:
		friend OwnerType;

	protected:
		TEventActionReadWrite();

	public:
		~TEventActionReadWrite() override;

	// ----------------------------------------------------
	// Event API

	protected:
		void Broadcast(TArgs const&... args);
	};
}

// --------------------------------------------------------

namespace aiva::utils_ext
{
	template <typename TOwnerType, typename... TArgs>
	TEventActionReadWrite<TOwnerType, TArgs...>::TEventActionReadWrite()
	{

	}

	template <typename TOwnerType, typename... TArgs>
	TEventActionReadWrite<TOwnerType, TArgs...>::~TEventActionReadWrite()
	{

	}

	template <typename TOwnerType, typename... TArgs>
	void TEventActionReadWrite<TOwnerType, TArgs...>::Broadcast(TArgs const&... args)
	{
		Event()(std::forward<TArgs const>(args)...);
	}
}
