#include <pch.h>
#include <aiva2/mat_lib.hpp>

#include <aiva2/material_base.hpp>

namespace aiva2
{
    auto mat_lib_t::get_instance() -> mat_lib_t&
    {
        static auto instance = mat_lib_t{};
        return instance;
    }

    auto mat_lib_t::copy(std::shared_ptr<material_base_t> const& mat) -> std::shared_ptr<material_base_t>
    {
        assert_t::check_bool(mat, "(mat) is not valid");

        auto const key = copy_key_type{ typeid(*mat) };
        auto const val = get_instance().m_copy_funcs.find(key);
        assert_t::check_bool(val != std::cend(get_instance().m_copy_funcs), "(val) is not found");

        return (*val).second(mat);
    }
}
