#pragma once
#include <pch.h>

#include <aiva2/engine/t_event_action_readonly.h>

namespace aiva2::engine
{
	template <typename TOwnerType, typename... TArgs>
	struct TEventActionReadWrite : public TEventActionReadOnly<TArgs...>
	{
	// ----------------------------------------------------

	public:
		using OwnerType = TOwnerType;

		friend OwnerType;

	protected:
		TEventActionReadWrite();

		~TEventActionReadWrite() override;

	// ----------------------------------------------------

	protected:
		void Broadcast(TArgs const&... args);
	};
}

// --------------------------------------------------------

namespace aiva2::engine
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
