#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/hlsl_semantic_type.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    constexpr void from_string(std::string_view const& str, hlsl_semantic_type_t& val)
    {
        if (str == "UNKNOWN")
            { val = hlsl_semantic_type_t::UNKNOWN; return; }

        if (str == "POSITION")
            { val = hlsl_semantic_type_t::POSITION; return; }

        if (str == "NORMAL")
            { val = hlsl_semantic_type_t::NORMAL; return; }

        if (str == "TEXCOORD")
            { val = hlsl_semantic_type_t::TEXCOORD; return; }

        if (str == "COLOR")
            { val = hlsl_semantic_type_t::COLOR; return; }

        if (str == "SV_POSITION")
            { val = hlsl_semantic_type_t::SV_POSITION; return; }

        if (str == "SV_TARGET")
            { val = hlsl_semantic_type_t::SV_TARGET; return; }

        if (str == "MAXIMUM")
            { val = hlsl_semantic_type_t::MAXIMUM; return; }

        assert_t::check_bool(false, "failed to convert string to hlsl semantic type");
        val = hlsl_semantic_type_t::UNKNOWN;
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
		from_string(json.get<std::string>(), type);
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
