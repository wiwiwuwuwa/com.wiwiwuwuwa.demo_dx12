#pragma once
#include <aiva/event_action_read_only.hpp>

namespace aiva
{
	template <typename t_owner, typename... t_args>
	struct event_action_read_write_t final : public event_action_read_only_t<t_args...>
	{
		// ------------------------------------------------

	protected:
		using owner_type = t_owner;

		friend owner_type;

		// ------------------------------------------------

	protected:
		event_action_read_write_t();

		~event_action_read_write_t() override;

		// ------------------------------------------------

	protected:
		void invoke(t_args&&... args) const;

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva
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
	void event_action_read_write_t<t_owner, t_args...>::invoke(t_args&&... args) const
	{
		(*this).get_event()(std::forward<t_args>(args)...);
	}
}
