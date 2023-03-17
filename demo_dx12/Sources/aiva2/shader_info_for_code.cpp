#include <pch.h>
#include <aiva2/shader_info_for_code.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/shader_info_for_resource.hpp>

namespace aiva2
{
	shader_info_for_code_t::shader_info_for_code_t(engine_t& engine, std::string const& text)
		: impl_type{ engine }
		, m_text{ text }
	{
		init_resources();
	}

	shader_info_for_code_t::~shader_info_for_code_t()
	{
		shut_resources();
	}

	auto shader_info_for_code_t::get_text() const->std::string const&
	{
		return m_text;
	}

	auto shader_info_for_code_t::has_text() const->bool
	{
		return !std::empty(m_text);
	}

	auto shader_info_for_code_t::get_resource(size_t const index) const->shader_info_for_resource_t const&
	{
		assert_t::check_bool(index >= size_t{} && index < std::size(m_resources), "index is not valid");

		auto const& resource = m_resources[index];
		assert_t::check_bool(resource, "resource not valid");

		return (*resource);
	}

	auto shader_info_for_code_t::num_resource() const->size_t
	{
		return std::size(m_resources);
	}

	void shader_info_for_code_t::init_resources()
	{
		m_resources = {};

		auto const regex_for_resources = std::regex(R"(\b\w+?\s*?(<[\s\S]*?>)?\s*?\w+?(\s*?\[[\s\S]*?\]\s*?)*?\s*?:\s*?register\s*?\([\s\S]*?\)\s*?;)", std::regex::icase | std::regex::optimize);

		for (auto i = std::sregex_iterator{ std::cbegin(m_text), std::cend(m_text), regex_for_resources }; i != decltype(i){}; i++)
		{
			auto const& resource_match = (*i);
			assert_t::check_bool(resource_match.ready(), "resource_match is not valid");
			assert_t::check_bool(!std::empty(resource_match), "resource_match is not valid");

			auto const resource_string = resource_match.str();
			assert_t::check_bool(!std::empty(resource_string), "resource_string is not valid");
			
			auto const& resource_info = m_resources.emplace_back(std::make_shared<shader_info_for_resource_t>(get_engine(), resource_string));
			assert_t::check_bool(resource_info, "resource_info is not valid");
		}
	}

	void shader_info_for_code_t::shut_resources()
	{
		m_resources = {};
	}
}
