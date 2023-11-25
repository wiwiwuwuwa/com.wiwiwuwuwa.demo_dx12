#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/struct.hpp>
#include <aiva2/vert_attr_type.hpp>

namespace aiva2
{
    struct vert_attr_pair_t final : public implements_t<vert_attr_pair_t, struct_t>
    {
        // ------------------------------------------------

    public:
        auto get_type() const->vert_attr_type_t;

        void set_type(vert_attr_type_t const type);

    private:
        vert_attr_type_t m_type{};

        // ------------------------------------------------

    public:
        auto get_indx() const->std::size_t;

        void set_indx(std::size_t const indx);

    private:
        std::size_t m_indx{};

        // ------------------------------------------------
    };
}
