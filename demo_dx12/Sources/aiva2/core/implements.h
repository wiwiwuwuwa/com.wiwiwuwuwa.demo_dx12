#pragma once
#include <pch.h>

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

        // ------------------------------------------------

    protected:
        virtual ~implements_t() = default;

        // ------------------------------------------------
    };

    template <typename t_this, typename t_base>
    struct implements_t<t_this, t_base> : public implements_t<t_this>
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
