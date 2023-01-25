#pragma once
#include <pch.h>

namespace aiva2::core
{
	template <typename... t_args>
	struct event_action_base_t : private boost::noncopyable
	{
		// ------------------------------------------------

	protected:
		event_action_base_t();

		virtual ~event_action_base_t();

		// ------------------------------------------------

	protected:
		using event_type = boost::signals2::signal<void(t_args...)>;

	protected:
		auto get_event() const -> event_type const&;

		auto get_event() -> event_type&;

	private:
		event_type event{};

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2::core
{
	template <typename... t_args>
	event_action_base_t<t_args...>::event_action_base_t()
	{

	}

	template <typename... t_args>
	event_action_base_t<t_args...>::~event_action_base_t()
	{

	}

	template <typename... t_args>
	auto event_action_base_t<t_args...>::get_event() const -> event_type const&
	{
		return event;
	}

	template <typename... t_args>
	auto event_action_base_t<t_args...>::get_event() -> event_type&
	{
		return event;
	}
}
