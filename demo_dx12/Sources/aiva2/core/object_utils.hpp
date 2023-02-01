#pragma once
#include <aiva2/base.hpp>

namespace aiva2::core
{
	template <typename t_obj, typename... t_args>
	auto new_object(t_args&&... args);
}

// --------------------------------------------------------

namespace aiva2::core
{
	template <typename t_obj, typename... t_args>
	auto new_object(t_args&&... args)
	{
		return std::shared_ptr<t_obj>{ new t_obj{ std::forward<t_args>(args)... }};
	}
}
