#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/event_action_read_write.hpp>

namespace aiva2::core
{
	template <typename... t_args>
	using event_action_for_ret_t = event_action_read_only_t<t_args...>;

	template <typename t_owner, typename... t_args>
	using event_action_for_var_t = event_action_read_write_t<t_owner, t_args...>;
}
