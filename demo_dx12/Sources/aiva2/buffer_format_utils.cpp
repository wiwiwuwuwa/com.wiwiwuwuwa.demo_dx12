#include <pch.h>
#include <aiva2/buffer_format_utils.hpp>

namespace aiva2
{
    void to_json(nlohmann::json& json, buffer_format_t const& format)
    {
        json = nlohmann::json{ to_string(format) };
    }

    void from_json(nlohmann::json const& json, buffer_format_t& format)
    {
		from_string(json.get<std::string>(), format);
    }
}
