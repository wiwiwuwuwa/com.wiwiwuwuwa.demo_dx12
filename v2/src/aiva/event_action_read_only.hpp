#pragma once
#include <aiva/event_action_base.hpp>

namespace aiva
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
		void detach_listener(t_bind_args&&... args);

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

#include <boost/bind/bind.hpp>

namespace aiva
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
		(*this).get_event().connect(boost::bind(std::forward<t_bind_args>(args)...));
	}

	template <typename... t_args>
	template <typename... t_bind_args>
	void event_action_read_only_t<t_args...>::detach_listener(t_bind_args&&... args)
	{
		(*this).get_event().disconnect(boost::bind(std::forward<t_bind_args>(args)...));
	}
}
