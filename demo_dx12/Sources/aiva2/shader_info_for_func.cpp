#include <pch.h>
#include <aiva2/shader_info_for_func.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>

namespace aiva2
{
    shader_info_for_func_t::shader_info_for_func_t(engine_t &engine, std::string_view const& text)
        : impl_type{ engine }
        , m_text{ text }
    {
        init_self();
    }

    shader_info_for_func_t::~shader_info_for_func_t()
    {
        shut_self();
    }

    auto shader_info_for_func_t::get_text() const -> std::string const&
    {
        return m_text;
    }

    auto shader_info_for_func_t::get_name() const -> std::string const&
    {
        return m_name;
    }

    // void shader_info_for_struct_t::init_name()
    // {
    //     auto const regex = std::regex{ R"(^\s*?struct\s+?(\w+))", std::regex::icase | std::regex::optimize };
    //     auto match = std::smatch{};

    //     assert_t::check_bool(std::regex_search(m_text, match, regex), "failed to find (name)");
    //     assert_t::check_bool(match.ready(), "failed to find (name)");
    //     assert_t::check_bool(std::size(match) == 2, "failed to find (name)");

    //     auto const name_match = match[1];
    //     assert_t::check_bool(name_match.matched, "(name_match) is not valid");

    //     auto const name_string = name_match.str();
    //     assert_t::check_bool(!std::empty(name_string), "(name_string) is not valid");

    //     m_name = name_string;
    // }

    void shader_info_for_func_t::init_self()
    {
        assert_t::check_bool(false, "puk");
    }

	void shader_info_for_func_t::shut_self()
	{
		assert_t::check_bool(false, "puk");
	}

    auto shader_info_for_func_t::get_input_type(size_t const index) const -> std::string const&
    {
        assert_t::check_bool(index >= decltype(index){}, "(index) is not valid");
        assert_t::check_bool(index < std::size(m_input_types), "(index) is not valid");

        return m_input_types[index];
    }

    auto shader_info_for_func_t::num_input_type() const -> size_t
    {
        return std::size(m_input_types);
    }

    auto shader_info_for_func_t::get_output_type() const -> std::string const&
    {
        return m_output_type;
    }
}