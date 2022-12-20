#pragma once
#include <pch.h>

#include <aiva2/engine/m_field_by_val.h>

namespace aiva2::engine
{
	template <typename... TArgs>
	struct TEventActionBase : private boost::noncopyable
	{
	// ----------------------------------------------------

	public:
		using EventType = boost::signals2::signal<void(TArgs...)>;

	protected:
		TEventActionBase();

		virtual ~TEventActionBase();

	// ----------------------------------------------------

	public:
		M_FIELD_BY_VAL_3(protected, EventType, Event);
	};
}

// --------------------------------------------------------

namespace aiva2::engine
{
	template <typename... TArgs>
	TEventActionBase<TArgs...>::TEventActionBase()
	{

	}

	template <typename... TArgs>
	TEventActionBase<TArgs...>::~TEventActionBase()
	{

	}
}
