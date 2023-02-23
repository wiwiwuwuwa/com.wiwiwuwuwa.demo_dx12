#pragma once
#include <aiva2/_core.hpp>
#include <aiva2/event_action.hpp>

namespace aiva2
{
	template <typename... t_interfaces>
	struct implements_t;

	template <typename t_this>
	struct implements_t<t_this>
	{
		// ------------------------------------------------

	public:
		using impl_type = implements_t<t_this>;

		using this_type = t_this;

	public:
		template <typename... t_args>
		using event_action_type = event_action_for_var_t<t_this, t_args...>;

	protected:
		implements_t() = default;

		virtual ~implements_t() = default;

		// ------------------------------------------------
	};

	template <typename t_this, typename t_base>
	struct implements_t<t_this, t_base> : public implements_t<t_this>, public t_base
	{
		// ------------------------------------------------

	public:
		using base_type = t_base;

		using impl_type = implements_t<t_this, t_base>;

		using this_type = t_this;

	public:
		template <typename... t_args>
		using event_action_type = event_action_for_var_t<t_this, t_args...>;

	protected:
		template <typename... t_args>
		implements_t(t_args&&... args) : t_base{ std::forward<t_args>(args)... } {};

		// ------------------------------------------------
	};

	template <typename t_this, typename t_base, typename... t_interfaces>
	struct implements_t<t_this, t_base, t_interfaces...> : public implements_t<t_this, t_base>, public t_interfaces...
	{
		// ------------------------------------------------

	public:
		using impl_type = implements_t<t_this, t_base, t_interfaces...>;

		// ------------------------------------------------
	};
}
