#pragma once
#include <pch.h>

#include <aiva2/core/event_action_read_only.h>

namespace aiva2::core
{
	template <typename t_owner, typename... t_args>
	struct event_action_read_write_t : public event_action_read_only_t<t_args...>
	{
		// ------------------------------------------------

	protected:
		using owner_type = t_owner;

		friend owner_type;

	protected:
		event_action_read_write_t();

		~event_action_read_write_t() override;

		// ------------------------------------------------

	protected:
		void broadcast(t_args const&... args);

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2::core
{
	template <typename t_owner, typename... t_args>
	event_action_read_write_t<t_owner, t_args...>::event_action_read_write_t()
	{

	}

	template <typename t_owner, typename... t_args>
	event_action_read_write_t<t_owner, t_args...>::~event_action_read_write_t()
	{

	}

	template <typename t_owner, typename... t_args>
	void event_action_read_write_t<t_owner, t_args...>::broadcast(t_args const&... args)
	{
		get_event()(std::forward<t_args const>(args)...);
	}
}
