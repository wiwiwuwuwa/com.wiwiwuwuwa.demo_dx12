#pragma once
#include <pch.h>

#include <aiva2/core/event_action_base.h>

namespace aiva2::core
{
	template <typename... t_args>
	struct event_action_read_only_t : public event_action_base_t<t_args...>
	{
		// ------------------------------------------------

	protected:
		event_action_read_only_t();

		~event_action_read_only_t() override;

		// ------------------------------------------------

	public:
		template <typename... t_bind_args>
		void attach_listener(t_bind_args&&... args);

		template <typename... t_bind_args>
		void remove_listener(t_bind_args&&... args);

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2::core
{
	template <typename... t_args>
	event_action_read_only_t<t_args...>::event_action_read_only_t()
	{

	}

	template <typename... t_args>
	event_action_read_only_t<t_args...>::~event_action_read_only_t()
	{

	}

	template <typename... t_args>
	template <typename... t_bind_args>
	void event_action_read_only_t<t_args...>::attach_listener(t_bind_args&&... args)
	{
		get_event().connect(boost::bind(std::forward<t_bind_args>(args)...));
	}

	template <typename... t_args>
	template <typename... t_bind_args>
	void event_action_read_only_t<t_args...>::remove_listener(t_bind_args&&... args)
	{
		get_event().disconnect(boost::bind(std::forward<t_bind_args>(args)...));
	}
}
