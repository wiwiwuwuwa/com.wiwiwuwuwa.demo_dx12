#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/parse_enum.hpp>
#include <aiva2/hlsl_semantic_type.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    template <>
    constexpr auto parse_enum_impl<parse_enum_wrapper_t<hlsl_semantic_type_t>>(std::string_view const& str) -> hlsl_semantic_type_t
    {
        if (str == "UNKNOWN")
            return hlsl_semantic_type_t::UNKNOWN;

        if (str == "POSITION")
            return hlsl_semantic_type_t::POSITION;

        if (str == "NORMAL")
            return hlsl_semantic_type_t::NORMAL;

        if (str == "TEXCOORD")
            return hlsl_semantic_type_t::TEXCOORD;

        if (str == "COLOR")
            return hlsl_semantic_type_t::COLOR;

        if (str == "SV_POSITION")
            return hlsl_semantic_type_t::SV_POSITION;

        if (str == "SV_TARGET")
            return hlsl_semantic_type_t::SV_TARGET;

        if (str == "MAXIMUM")
            return hlsl_semantic_type_t::MAXIMUM;

        assert_t::check_bool(false, "failed to convert string to hlsl semantic type");
        return hlsl_semantic_type_t::UNKNOWN;
    }

    constexpr auto to_string(hlsl_semantic_type_t const type)
    {
        if (type == hlsl_semantic_type_t::UNKNOWN)
            return "UNKNOWN";

        if (type == hlsl_semantic_type_t::POSITION)
            return "POSITION";

        if (type == hlsl_semantic_type_t::NORMAL)
            return "NORMAL";

        if (type == hlsl_semantic_type_t::TEXCOORD)
            return "TEXCOORD";

        if (type == hlsl_semantic_type_t::COLOR)
            return "COLOR";

        if (type == hlsl_semantic_type_t::SV_POSITION)
            return "SV_POSITION";

        if (type == hlsl_semantic_type_t::SV_TARGET)
            return "SV_TARGET";

        if (type == hlsl_semantic_type_t::MAXIMUM)
            return "MAXIMUM";

        assert_t::check_bool(false, "failed to convert hlsl semantic type to string");
        return "UNKNOWN";
    }

    // ----------------------------------------------------

    void to_json(nlohmann::json& json, hlsl_semantic_type_t const& type)
    {
        json = nlohmann::json{ to_string(type) };
    }

    void from_json(nlohmann::json const& json, hlsl_semantic_type_t& type)
    {
		type = parse_enum<hlsl_semantic_type_t>(json.get<std::string>());
    }

    // ----------------------------------------------------

    constexpr auto is_input_assembly(hlsl_semantic_type_t const type)
    {
        if (type == hlsl_semantic_type_t::POSITION)
            return true;

        if (type == hlsl_semantic_type_t::NORMAL)
            return true;

        if (type == hlsl_semantic_type_t::TEXCOORD)
            return true;

        if (type == hlsl_semantic_type_t::COLOR)
            return true;

        return false;
    }

    constexpr auto is_system_value(hlsl_semantic_type_t const type)
    {
        if (type == hlsl_semantic_type_t::SV_POSITION)
            return true;

        if (type == hlsl_semantic_type_t::SV_TARGET)
            return true;

        return false;
    }

    // ----------------------------------------------------
}
