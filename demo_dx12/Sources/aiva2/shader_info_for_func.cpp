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

    void shader_info_for_func_t::init_self()
    {
        auto const regex = std::regex{ R"((?:^|\n)\s*(\w+)\s+(\w+)\s*\(([\s\S]*?)\))", std::regex::icase | std::regex::optimize };
        auto match = std::smatch{};

        assert_t::check_bool(std::regex_search(m_text, match, regex), "failed to (regex_search)");
        assert_t::check_bool(match.ready(), "failed to (regex_search)");
        assert_t::check_bool(std::size(match) == 4, "failed to (regex_search)");

        {
            auto const name_match = match[2];
            assert_t::check_bool(name_match.matched, "(name_match) is not valid");

            auto const name_string = name_match.str();
            assert_t::check_bool(!std::empty(name_string), "(name_string) is not valid");

            m_name = name_string;
        }

        {
            m_input_types = {};

            auto const input_types_match = match[3];
            assert_t::check_bool(input_types_match.matched, "(input_types_match) is not valid");

            auto const input_types_string = input_types_match.str();
            auto const input_type_regex = std::regex{ R"(\b(\w+)\b\s*\b\w+\s*?(?:$|\n|,))", std::regex::icase | std::regex::optimize };

            for (auto i = std::sregex_iterator{ std::cbegin(input_types_string), std::cend(input_types_string), input_type_regex }; i != decltype(i){}; i++)
            {
                auto const& input_type_match = (*i);
                assert_t::check_bool(input_type_match.ready(), "(input_type_match) is not valid");
                assert_t::check_bool(std::size(input_type_match) == 2, "(input_type_match) is not valid");

                auto const input_type_string = input_type_match.str();
                assert_t::check_bool(!std::empty(input_type_string), "(input_type_string) is not valid");

                m_input_types.emplace_back(input_type_string);
            }
        }

        {
            auto const output_type_match = match[1];
            assert_t::check_bool(output_type_match.matched, "(output_type_match) is not valid");

            auto const output_type_string = output_type_match.str();
            assert_t::check_bool(!std::empty(output_type_string), "(output_type_string) is not valid");

            m_output_type = output_type_string;
        }
    }

	void shader_info_for_func_t::shut_self()
	{
		m_output_type = {};
        m_input_types = {};
        m_name = {};
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