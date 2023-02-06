#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/event_action.hpp>

namespace aiva2::core
{
    template <typename... t_interfaces>
    struct implements_t;

    template <typename t_this>
    struct implements_t<t_this>
    {
        // ------------------------------------------------

    public:
        using this_type = t_this;

        template <typename... t_args>
        using event_action_type = event_action_for_var_t<this_type, t_args...>;

        // ------------------------------------------------

    protected:
        virtual ~implements_t() = default;

        // ------------------------------------------------
    };

    template <typename t_this, typename t_base>
	struct implements_t<t_this, t_base> : public implements_t<t_this>, public t_base
    {
        // ------------------------------------------------

    public:
        using base_type = t_base;

        // ------------------------------------------------
    };

    template <typename t_this, typename t_base, typename... t_interfaces>
    struct implements_t<t_this, t_base, t_interfaces...> : public implements_t<t_this, t_base>, public t_interfaces...
    {
    };
}
