#include <pch.h>
#include <aiva2/shader_info_for_resource.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/enum_utils.hpp>
#include <aiva2/shader_register_type_utils.hpp>
#include <aiva2/shader_resource_type_utils.hpp>

namespace aiva2
{
	shader_info_for_resource_t::shader_info_for_resource_t(engine_t& engine, std::string const& text)
		: impl_type{ engine }
		, m_text{ text }
	{
		init_type();
		init_name();
		init_register();
	}

	shader_info_for_resource_t::~shader_info_for_resource_t()
	{
		shut_register();
		shut_name();
		shut_type();
	}

	auto shader_info_for_resource_t::get_text() const->std::string const&
	{
		return m_text;
	}

	auto shader_info_for_resource_t::has_text() const->bool
	{
		return !std::empty(m_text);
	}

	auto shader_info_for_resource_t::get_type() const->shader_resource_type_t
	{
		return m_type;
	}
	
	void shader_info_for_resource_t::init_type()
	{
		auto const regex_for_type = std::regex{ R"(^\s*?(\w+?)\b)", std::regex::icase | std::regex::optimize };
		auto match_for_type = std::smatch{};

		assert_t::check_bool(std::regex_search(m_text, match_for_type, regex_for_type), "failed to find type");
		assert_t::check_bool(match_for_type.ready(), "failed to find type");
		assert_t::check_bool(std::size(match_for_type) == 2, "failed to find type");

		auto const type_match = match_for_type[1];
		assert_t::check_bool(type_match.matched, "type_match is not valid");

		auto const type_string = type_match.str();
		assert_t::check_bool(!std::empty(type_string), "type_string is not valid");

		auto type_enum = shader_resource_type_t{}; from_hlsl(type_string, type_enum);
		assert_t::check_bool(is_valid(type_enum), "type_enum is not valid");

		m_type = type_enum;
	}

	void shader_info_for_resource_t::shut_type()
	{
		m_type = {};
	}
	
	auto shader_info_for_resource_t::get_name() const->std::string const&
	{
		return m_name;
	}

	auto shader_info_for_resource_t::has_name() const->bool
	{
		return !std::empty(m_name);
	}

	void shader_info_for_resource_t::init_name()
	{
		auto const regex_for_name = std::regex{ R"(\b(\w+?)\s*?(?:\[[\s\S]*?\])?\s*?:)", std::regex::icase | std::regex::optimize };
		auto match_for_name = std::smatch{};

		assert_t::check_bool(std::regex_search(m_text, match_for_name, regex_for_name), "failed to find name");
		assert_t::check_bool(match_for_name.ready(), "failed to find name");
		assert_t::check_bool(std::size(match_for_name) == 2, "failed to find name");

		auto const& name_match = match_for_name[1];
		assert_t::check_bool(name_match.matched, "name_match is not valid");

		m_name = name_match.str();
		assert_t::check_bool(!std::empty(m_name), "name is not valid");
	}

	void shader_info_for_resource_t::shut_name()
	{
		m_name.clear();
	}

	auto shader_info_for_resource_t::get_register_type() const->shader_register_type_t
	{
		return m_register_type;
	}

	auto shader_info_for_resource_t::get_register_index() const->size_t
	{
		return m_register_index;
	}

	auto shader_info_for_resource_t::get_register_space() const->size_t
	{
		return m_register_space;
	}

	void shader_info_for_resource_t::init_register()
	{
		auto const regex_for_register = std::regex{ R"(\(\s*?(\w)(\d+?)\s*?(?:,\s*?space(\d+?)\s*?)?\))", std::regex::icase | std::regex::optimize };
		auto match_for_register = std::smatch{};

		assert_t::check_bool(std::regex_search(m_text, match_for_register, regex_for_register), "failed to find register");
		assert_t::check_bool(match_for_register.ready(), "failed to find register");
		assert_t::check_bool(std::size(match_for_register) == 4, "failed to find register");

		{
			auto const& register_type_match = match_for_register[1];
			assert_t::check_bool(register_type_match.matched, "register_type_match is not valid");

			auto const register_type_string = register_type_match.str();
			assert_t::check_bool(std::size(register_type_string) == 1, "register_type_string is not valid");

			auto register_type_enum = shader_register_type_t{}; from_letter(register_type_string[0], register_type_enum);
			assert_t::check_bool(is_valid(register_type_enum), "register_type_enum is not valid");
			
			m_register_type = register_type_enum;
		}

		{
			auto const& register_index_match = match_for_register[2];
			assert_t::check_bool(register_index_match.matched, "register_index_match is not valid");

			auto const register_index_as_string = register_index_match.str();
			assert_t::check_bool(!std::empty(register_index_as_string), "register_index_as_string is not valid");
			
			auto const register_index_as_ulong = std::stoull(register_index_as_string);
			assert_t::check_bool(register_index_as_ulong <= std::numeric_limits<size_t>::max(), "register_index_as_ulong is too big");

			m_register_index = static_cast<size_t>(register_index_as_ulong);
			assert_t::check_bool(m_register_index == register_index_as_ulong, "register_index is not valid");
		}

		{
			auto const& register_space_match = match_for_register[3];
			if (register_space_match.matched)
			{
				auto const register_space_as_string = register_space_match.str();
				assert_t::check_bool(!std::empty(register_space_as_string), "register_space_as_string is not valid");

				auto const register_space_as_ulong = std::stoull(register_space_as_string);
				assert_t::check_bool(register_space_as_ulong <= std::numeric_limits<size_t>::max(), "register_space_as_ulong is too big");
				
				m_register_space = static_cast<size_t>(register_space_as_ulong);
				assert_t::check_bool(m_register_space == register_space_as_ulong, "register_space is not valid");
			}
			else
			{
				m_register_space = {};
			}
		}
	}

	void shader_info_for_resource_t::shut_register()
	{
		m_register_space = {};
		m_register_index = {};
		m_register_type = {};
	}
}
