#pragma once
#include <pch.h>

#include <aiva/utils_ext/m_object_field_val.h>

namespace aiva::utils_ext
{
	template <typename... TArgs>
	struct TEventActionBase : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Main

	public:
		using EventType = boost::signals2::signal<void(TArgs...)>;

	protected:
		TEventActionBase();

	public:
		virtual ~TEventActionBase();

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_VAL_4(EventType, Event, protected, protected);
	};
}

// --------------------------------------------------------

namespace aiva::utils_ext
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
