#include <pch.h>
#include <aiva2/shader_info_for_struct.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/hlsl_semantic_type_utils.hpp>
#include <aiva2/shader_info_for_struct_field.hpp>

namespace aiva2
{
    shader_info_for_struct_t::shader_info_for_struct_t(engine_t &engine, std::string_view const& text)
        : impl_type{ engine }
        , m_text{ text }
    {
        init_name();
        init_fields();
        init_input_layout_slot_indices();
        init_byte_layout();
    }

    shader_info_for_struct_t::~shader_info_for_struct_t()
    {
        shut_byte_layout();
        shut_input_layout_slot_indices();
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

    auto shader_info_for_struct_t::get_field_idx(std::string_view const& name) const -> std::optional<size_t>
    {
        if (std::empty(name)) return {};

        auto const field_itr = std::find_if(std::cbegin(m_fields), std::cend(m_fields), [&](auto const& field_ptr)
        {
            return field_ptr && (*field_ptr).get_name() == name;
        });
        if (field_itr == std::cend(m_fields)) return {};

        auto const field_idx = static_cast<size_t>(std::distance(std::cbegin(m_fields), field_itr));
        if (field_idx < decltype(field_idx){}) return {};
        if (field_idx >= std::size(m_fields)) return {};

        return field_idx;
    }

    auto shader_info_for_struct_t::get_field_idx(hlsl_semantic_type_t const& semantic_type, size_t const semantic_index) const -> std::optional<size_t>
    {
        if (!is_valid(semantic_type)) return {};
        if (semantic_index < decltype(semantic_index){}) return {};

        auto const field_itr = std::find_if(std::cbegin(m_fields), std::cend(m_fields), [&](auto const& field_ptr)
        {
            return field_ptr && (*field_ptr).get_semantic_type() == semantic_type && (*field_ptr).get_semantic_index() == semantic_index;
        });
        if (field_itr == std::cend(m_fields)) return {};

        auto const field_idx = static_cast<size_t>(std::distance(std::cbegin(m_fields), field_itr));
        if (field_idx < decltype(field_idx){}) return {};
        if (field_idx >= std::size(m_fields)) return {};

        return field_idx;
    }

    auto shader_info_for_struct_t::get_field_idx(shader_info_for_struct_field_t const& field) const -> std::optional<size_t>
    {
        auto const field_itr = std::find_if(std::cbegin(m_fields), std::cend(m_fields), [&](auto const& field_ptr)
        {
            return field_ptr && field_ptr.get() == &field;
        });
        if (field_itr == std::cend(m_fields)) return {};

        auto const field_idx = static_cast<size_t>(std::distance(std::cbegin(m_fields), field_itr));
        if (field_idx < decltype(field_idx){}) return {};
        if (field_idx >= std::size(m_fields)) return {};

        return field_idx;
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

    auto shader_info_for_struct_t::get_field_ref(hlsl_semantic_type_t const& semantic_type, size_t const semantic_index) const -> shader_info_for_struct_field_t const&
    {
        auto const& field_ptr = get_field_ptr(semantic_type, semantic_index);
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
    
    auto shader_info_for_struct_t::get_field_ptr(hlsl_semantic_type_t const& semantic_type, size_t const semantic_index) const -> std::shared_ptr<shader_info_for_struct_field_t const>
    {
        if (!is_valid(semantic_type)) return {};
        if (semantic_index < decltype(semantic_index){}) return {};

        auto const field_itr = std::find_if(std::cbegin(m_fields), std::cend(m_fields), [&](auto const& field_ptr)
        {
            assert_t::check_bool(field_ptr, "(field_ptr) is not valid");
            return (*field_ptr).get_semantic_type() == semantic_type && (*field_ptr).get_semantic_index() == semantic_index;
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
        auto byte_offset = size_t{};

        for (auto i = std::sregex_iterator{ std::cbegin(m_text), std::cend(m_text), regex }; i != decltype(i){}; i++)
        {
            auto const& field_match = (*i);
            assert_t::check_bool(field_match.ready(), "(field_match) is not valid");
            assert_t::check_bool(std::size(field_match) == 2, "(field_match) is not valid");

            auto const field_string = field_match.str();
            assert_t::check_bool(!std::empty(field_string), "(field_string) is not valid");

            auto const& field_info = m_fields.emplace_back(std::make_shared<shader_info_for_struct_field_t>(get_engine(), field_string, byte_offset));
            assert_t::check_bool(field_info, "field_info is not valid");

            if (!(*field_info).has_byte_layout()) continue;

            byte_offset = (*field_info).get_byte_offset() + (*field_info).get_byte_size();
        }
    }

    void shader_info_for_struct_t::shut_fields()
    {
        m_fields = {};
    }

    auto shader_info_for_struct_t::get_input_layout_slot(size_t const field_index) const -> std::optional<size_t>
    {
        auto const iter = std::find(std::cbegin(m_slot_index_to_field_index), std::cend(m_slot_index_to_field_index), field_index);
        if (iter == std::cend(m_slot_index_to_field_index)) return {};

        auto const slot = static_cast<size_t>(std::distance(std::cbegin(m_slot_index_to_field_index), iter));
        if (slot < decltype(slot){}) return {};
        if (slot >= std::size(m_slot_index_to_field_index)) return {};

        return slot;
    }

    auto shader_info_for_struct_t::get_input_layout_slot(std::string_view const& field_name) const -> std::optional<size_t>
    {
        auto const field_index = get_field_idx(field_name);
        if (!field_index) return {};

        return get_input_layout_slot(*field_index);
    }

    auto shader_info_for_struct_t::get_input_layout_slot(hlsl_semantic_type_t const& semantic_type, size_t const semantic_index) const -> std::optional<size_t>
    {
        auto const field_index = get_field_idx(semantic_type, semantic_index);
        if (!field_index) return {};

        return get_input_layout_slot(*field_index);
    }

    auto shader_info_for_struct_t::get_input_layout_slot(shader_info_for_struct_field_t const& field) const -> std::optional<size_t>
    {
        auto const field_index = get_field_idx(field);
        if (!field_index) return {};

        return get_input_layout_slot(*field_index);
    }

    auto shader_info_for_struct_t::get_input_layout_field_ref(size_t const slot_index) const -> shader_info_for_struct_field_t const&
    {
        auto const field_ptr = get_input_layout_field_ptr(slot_index);
        assert_t::check_bool(field_ptr, "(field_ptr) is not valid");

        return (*field_ptr);
    }

    auto shader_info_for_struct_t::get_input_layout_field_ptr(size_t const slot_index) const -> std::shared_ptr<shader_info_for_struct_field_t const>
    {
        if (slot_index < decltype(slot_index){}) return {};
        if (slot_index >= std::size(m_slot_index_to_field_index)) return {};

        auto const field_index = m_slot_index_to_field_index[slot_index];
        if (field_index < decltype(field_index){}) return {};
        if (field_index >= num_field()) return {};

        return get_field_ptr(field_index);
    }

    auto shader_info_for_struct_t::num_input_layout_slot() const -> size_t
    {
        return std::size(m_slot_index_to_field_index);
    }

    void shader_info_for_struct_t::init_input_layout_slot_indices()
    {
        m_slot_index_to_field_index = {};
        
        for (auto i = size_t{}; i < num_field(); i++)
        {
            auto const& field = get_field_ref(i);

            auto const semantic_type = field.get_semantic_type();
            if (!is_input_assembly(semantic_type)) continue;

            m_slot_index_to_field_index.emplace_back(i);
        }
    }

    void shader_info_for_struct_t::shut_input_layout_slot_indices()
    {
        m_slot_index_to_field_index = {};
    }

    auto shader_info_for_struct_t::get_byte_size() const -> size_t
    {
        return m_byte_size;
    }

    void shader_info_for_struct_t::init_byte_layout()
    {
        m_byte_size = {};
        
        for (auto i = size_t{}; i < num_field(); i++)
        {
            auto const& field = get_field_ref(i);
            if (!field.has_byte_layout()) continue;

            m_byte_size = std::max(m_byte_size, field.get_byte_offset() + field.get_byte_size());
        }
    }

    void shader_info_for_struct_t::shut_byte_layout()
    {
        m_byte_size = {};
    }
}
