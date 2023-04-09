#include <pch.h>
#include <aiva2/shader_info_for_struct.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/shader_info_for_struct_field.hpp>

namespace aiva2
{
    shader_info_for_struct_t::shader_info_for_struct_t(engine_t &engine, std::string_view const& text)
        : impl_type{ engine }
        , m_text{ text }
    {
        init_name();
        init_fields();
    }

    shader_info_for_struct_t::~shader_info_for_struct_t()
    {
        shut_fields();
        shut_name();
    }

    auto shader_info_for_struct_t::get_text() const -> std::string const&
    {
        return m_text;
    }

     auto shader_info_for_struct_t::get_name() const -> std::string const&
    {
        return m_name;
    }

    void shader_info_for_struct_t::init_name()
    {
        auto const regex = std::regex{ R"(^\s*?struct\s+?(\w+))", std::regex::icase | std::regex::optimize };
        auto match = std::smatch{};

        assert_t::check_bool(std::regex_search(m_text, match, regex), "failed to find (name)");
        assert_t::check_bool(match.ready(), "failed to find (name)");
        assert_t::check_bool(std::size(match) == 2, "failed to find (name)");

        auto const name_match = match[1];
        assert_t::check_bool(name_match.matched, "(name_match) is not valid");

        auto const name_string = name_match.str();
        assert_t::check_bool(!std::empty(name_string), "(name_string) is not valid");

        m_name = name_string;
    }

    void shader_info_for_struct_t::shut_name()
    {
        m_name = {};
    }

    auto shader_info_for_struct_t::get_field_ref(size_t const index) const -> shader_info_for_struct_field_t const&
    {
        assert_t::check_bool(index >= decltype(index){}, "(index) is not valid");
        assert_t::check_bool(index < std::size(m_fields), "(index) is not valid");

        auto const& field_ptr = get_field_ptr(index);
        assert_t::check_bool(field_ptr, "(field_ptr) is not valid");

        return (*field_ptr);
    }

    auto shader_info_for_struct_t::get_field_ref(std::string_view const& name) const -> shader_info_for_struct_field_t const&
    {
        assert_t::check_bool(!name.empty(), "(name) is not valid");

        auto const& field_ptr = get_field_ptr(name);
        assert_t::check_bool(field_ptr, "(field_ptr) is not valid");

        return (*field_ptr);
    }

    auto shader_info_for_struct_t::get_field_ptr(size_t const index) const -> std::shared_ptr<shader_info_for_struct_field_t const>
    {
        assert_t::check_bool(index >= decltype(index){}, "(index) is not valid");
        assert_t::check_bool(index < std::size(m_fields), "(index) is not valid");

        return m_fields[index];
    }

    auto shader_info_for_struct_t::get_field_ptr(std::string_view const& name) const -> std::shared_ptr<shader_info_for_struct_field_t const>
    {
        assert_t::check_bool(!name.empty(), "(name) is not valid");

        auto const field_itr = std::find_if(std::cbegin(m_fields), std::cend(m_fields), [&](auto const& field_ptr)
        {
            assert_t::check_bool(field_ptr, "(field_ptr) is not valid");
            return (*field_ptr).get_name() == name;
        });
        
        return (field_itr != std::cend(m_fields)) ? (*field_itr) : nullptr;
    }

    auto shader_info_for_struct_t::num_field() const -> size_t
    {
        return std::size(m_fields);
    }

    void shader_info_for_struct_t::init_fields()
    {
        m_fields = {};

        auto const regex = std::regex{ R"([{\n]\s*\b([\s\S]*?;))", std::regex::icase | std::regex::optimize };

        for (auto i = std::sregex_iterator{ std::cbegin(m_text), std::cend(m_text), regex }; i != decltype(i){}; i++)
        {
            auto const& field_match = (*i);
            assert_t::check_bool(field_match.ready(), "(field_match) is not valid");
            assert_t::check_bool(std::size(field_match) == 2, "(field_match) is not valid");

            auto const field_string = field_match.str();
            assert_t::check_bool(!std::empty(field_string), "(field_string) is not valid");

            auto const& field_info = m_fields.emplace_back(std::make_shared<shader_info_for_struct_field_t>(get_engine(), field_string));
            assert_t::check_bool(field_info, "field_info is not valid");
        }
    }

    void shader_info_for_struct_t::shut_fields()
    {
        m_fields = {};
    }
}