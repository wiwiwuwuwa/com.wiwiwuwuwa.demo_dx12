#include <pch.h>
#include <aiva2/shader_info_for_meta.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>

namespace aiva2
{
	shader_info_for_meta_t::shader_info_for_meta_t(engine_t& engine, std::string const& text)
		: impl_type{ engine }
		, m_text{ text }
	{
		init_entry_for_comp();
		init_entry_for_vert();
		init_entry_for_frag();
	}

	shader_info_for_meta_t::~shader_info_for_meta_t()
	{
		shut_entry_for_frag();
		shut_entry_for_vert();
		shut_entry_for_comp();
	}

	auto shader_info_for_meta_t::get_text() const->std::string const&
	{
		return m_text;
	}

	auto shader_info_for_meta_t::has_text() const->bool
	{
		return !std::empty(m_text);
	}
	
	auto shader_info_for_meta_t::get_entry_for_comp() const->std::string const&
	{
		return m_entry_for_comp;
	}

	auto shader_info_for_meta_t::has_entry_for_comp() const->bool
	{
		return !std::empty(m_entry_for_comp);
	}

	void shader_info_for_meta_t::init_entry_for_comp()
	{
		auto const regex_for_entry_for_comp = std::regex{ R"(\bentry_for_comp\s*?=\s*?(\w+)\b)", std::regex::icase | std::regex::optimize };
		auto match_for_entry_for_comp = std::smatch{};

		if (std::regex_search(m_text, match_for_entry_for_comp, regex_for_entry_for_comp))
		{
			assert_t::check_bool(match_for_entry_for_comp.ready(), "failed to find entry_for_comp");
			assert_t::check_bool(std::size(match_for_entry_for_comp) == 2, "failed to find entry_for_comp");

			m_entry_for_comp = match_for_entry_for_comp[1].str();
		}
		else
		{
			m_entry_for_comp = {};
		}
	}

	void shader_info_for_meta_t::shut_entry_for_comp()
	{
		m_entry_for_comp = {};
	}

	auto shader_info_for_meta_t::get_entry_for_vert() const->std::string const&
	{
		return m_entry_for_vert;
	}

	auto shader_info_for_meta_t::has_entry_for_vert() const->bool
	{
		return !std::empty(m_entry_for_vert);
	}

	void shader_info_for_meta_t::init_entry_for_vert()
	{
		auto const regex_for_entry_for_vert = std::regex{ R"(\bentry_for_vert\s*?=\s*?(\w+)\b)", std::regex::icase | std::regex::optimize };
		auto match_for_entry_for_vert = std::smatch{};

		if (std::regex_search(m_text, match_for_entry_for_vert, regex_for_entry_for_vert))
		{
			assert_t::check_bool(match_for_entry_for_vert.ready(), "failed to find entry_for_vert");
			assert_t::check_bool(std::size(match_for_entry_for_vert) == 2, "failed to find entry_for_vert");

			m_entry_for_vert = match_for_entry_for_vert[1].str();
		}
		else
		{
			m_entry_for_vert = {};
		}
	}

	void shader_info_for_meta_t::shut_entry_for_vert()
	{
		m_entry_for_vert = {};
	}

	auto shader_info_for_meta_t::get_entry_for_frag() const->std::string const&
	{
		return m_entry_for_frag;
	}

	auto shader_info_for_meta_t::has_entry_for_frag() const->bool
	{
		return !std::empty(m_entry_for_frag);
	}

	void shader_info_for_meta_t::init_entry_for_frag()
	{
		auto const regex_for_entry_for_frag = std::regex{ R"(\bentry_for_frag\s*?=\s*?(\w+)\b)", std::regex::icase | std::regex::optimize };
		auto match_for_entry_for_frag = std::smatch{};

		if (std::regex_search(m_text, match_for_entry_for_frag, regex_for_entry_for_frag))
		{
			assert_t::check_bool(match_for_entry_for_frag.ready(), "failed to find entry_for_frag");
			assert_t::check_bool(std::size(match_for_entry_for_frag) == 2, "failed to find entry_for_frag");

			m_entry_for_frag = match_for_entry_for_frag[1].str();
		}
		else
		{
			m_entry_for_frag = {};
		}
	}

	void shader_info_for_meta_t::shut_entry_for_frag()
	{
		m_entry_for_frag = {};
	}
}
