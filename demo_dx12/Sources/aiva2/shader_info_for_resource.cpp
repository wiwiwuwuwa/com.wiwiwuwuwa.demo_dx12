#include <pch.h>
#include <aiva2/shader_info_for_resource.hpp>

#include <aiva2/engine.hpp>

namespace aiva2
{
	shader_info_for_resource_t::shader_info_for_resource_t(engine_t& engine, std::string const& text)
		: impl_type{ engine }
		, m_text{ text }
	{

	}

	shader_info_for_resource_t::~shader_info_for_resource_t()
	{

	}

	auto shader_info_for_resource_t::get_text() const->std::string const&
	{
		return m_text;
	}

	auto shader_info_for_resource_t::has_text() const->bool
	{
		return !std::empty(m_text);
	}
}
