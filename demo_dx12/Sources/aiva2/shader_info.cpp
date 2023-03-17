#include <pch.h>
#include <aiva2/shader_info.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/shader_info_for_code.hpp>
#include <aiva2/shader_info_for_meta.hpp>
#include <aiva2/string_utils.hpp>

namespace aiva2
{
	shader_info_t::shader_info_t(engine_t& engine, std::string const& text)
		: impl_type{ engine }
		, m_text{ string_utils_t::filter_comments(text) }
	{
		init_meta_block();
		init_code_block();
	}

	shader_info_t::~shader_info_t()
	{
		shut_code_block();
		shut_meta_block();
	}

	auto shader_info_t::get_text() const->std::string const&
	{
		return m_text;
	}

	auto shader_info_t::has_text() const->bool
	{
		return !std::empty(m_text);
	}

	auto shader_info_t::get_meta_block() const->shader_info_for_meta_t const&
	{
		assert_t::check_bool(m_meta_block, "m_meta_block is not valid");
		return (*m_meta_block);
	}

	auto shader_info_t::has_meta_block() const->bool
	{
		return m_meta_block != nullptr;
	}

	void shader_info_t::init_meta_block()
	{
		auto const regex_for_meta = std::regex{ R"((^|\n)\s*?#aiva\s+?init_meta_block\s*?\n([\s\S]*?)\n\s*?#aiva\s+?shut_meta_block\s*?($|\n))", std::regex::icase | std::regex::optimize };
		auto match_for_meta = std::smatch{};

		if (std::regex_search(m_text, match_for_meta, regex_for_meta))
		{
			assert_t::check_bool(match_for_meta.ready(), "failed to find meta block");
			assert_t::check_bool(std::size(match_for_meta) == 4, "failed to find meta block");
			
			m_meta_block = std::make_shared<shader_info_for_meta_t>(get_engine(), match_for_meta[2].str());
		}
		else
		{
			m_meta_block = {};
		}
	}

	void shader_info_t::shut_meta_block()
	{
		m_meta_block = {};
	}

	auto shader_info_t::get_code_block() const->shader_info_for_code_t const&
	{
		assert_t::check_bool(m_code_block, "m_code_block is not valid");
		return (*m_code_block);
	}

	auto shader_info_t::has_code_block() const->bool
	{
		return m_code_block != nullptr;
	}

	void shader_info_t::init_code_block()
	{
		auto const regex_for_code = std::regex{ R"((^|\n)\s*?#aiva\s+?init_code_block\s*?\n([\s\S]*?)\n\s*?#aiva\s+?shut_code_block\s*?($|\n))", std::regex::icase | std::regex::optimize };
		auto match_for_code = std::smatch{};

		if (std::regex_search(m_text, match_for_code, regex_for_code))
		{
			assert_t::check_bool(match_for_code.ready(), "failed to find code block");
			assert_t::check_bool(std::size(match_for_code) == 4, "failed to find code block");

			m_code_block = std::make_shared<shader_info_for_code_t>(get_engine(), match_for_code[2].str());
		}
		else
		{
			m_code_block = {};
		}
	}

	void shader_info_t::shut_code_block()
	{
		m_code_block = {};
	}
}
