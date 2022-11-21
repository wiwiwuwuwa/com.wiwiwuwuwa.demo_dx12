#pragma once
#include <pch.h>

#include <aiva/utils_ext/t_event_action_base.h>

namespace aiva::utils_ext
{
	template <typename... TArgs>
	struct TEventActionReadOnly : public TEventActionBase<TArgs...>
	{
	// ----------------------------------------------------
	// Main

	protected:
		TEventActionReadOnly();

	public:
		~TEventActionReadOnly() override;

	// ----------------------------------------------------
	// Event API

	public:
		template <typename... TBindArgs>
		void AttachListener(TBindArgs&&... args);

		template <typename... TBindArgs>
		void RemoveListener(TBindArgs&&... args);
	};
}

// --------------------------------------------------------

namespace aiva::utils_ext
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
