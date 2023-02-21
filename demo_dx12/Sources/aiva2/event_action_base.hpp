#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
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
		auto get_event() const->event_type const&;

		auto get_event() -> event_type&;

	private:
		event_type m_event{};

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2
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
		return m_event;
	}

	template <typename... t_args>
	auto event_action_base_t<t_args...>::get_event() -> event_type&
	{
		return m_event;
	}
}
