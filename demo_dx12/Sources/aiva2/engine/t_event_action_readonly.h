#pragma once
#include <pch.h>

#include <aiva2/engine/t_event_action_base.h>

namespace aiva2::engine
{
	template <typename... TArgs>
	struct TEventActionReadOnly : public TEventActionBase<TArgs...>
	{
	// ----------------------------------------------------

	protected:
		TEventActionReadOnly();

		~TEventActionReadOnly() override;

	// ----------------------------------------------------

	public:
		template <typename... TBindArgs>
		void AttachListener(TBindArgs&&... args);

		template <typename... TBindArgs>
		void RemoveListener(TBindArgs&&... args);
	};
}

// --------------------------------------------------------

namespace aiva2::engine
{
	template <typename... TArgs>
	TEventActionReadOnly<TArgs...>::TEventActionReadOnly()
	{

	}

	template <typename... TArgs>
	TEventActionReadOnly<TArgs...>::~TEventActionReadOnly()
	{

	}

	template <typename... TArgs>
	template <typename... TBindArgs>
	void TEventActionReadOnly<TArgs...>::AttachListener(TBindArgs&&... args)
	{
		Event().connect(boost::bind(std::forward<TBindArgs>(args)...));
	}

	template <typename... TArgs>
	template <typename... TBindArgs>
	void TEventActionReadOnly<TArgs...>::RemoveListener(TBindArgs&&... args)
	{
		Event().disconnect(boost::bind(std::forward<TBindArgs>(args)...));
	}
}
