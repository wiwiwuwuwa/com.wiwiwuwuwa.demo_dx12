#include <pch.h>
#include <aiva2/hlsl_semantic_type_utils.hpp>

namespace aiva2
{
    void to_json(nlohmann::json& json, hlsl_semantic_type_t const& type)
    {
        json = nlohmann::json{ to_string(type) };
    }

    void from_json(nlohmann::json const& json, hlsl_semantic_type_t& type)
    {
		type = parse_enum<hlsl_semantic_type_t>(json.get<std::string>());
    }
}
