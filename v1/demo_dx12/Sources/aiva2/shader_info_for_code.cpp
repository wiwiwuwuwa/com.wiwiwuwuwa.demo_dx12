#include <pch.h>
#include <aiva2/shader_info_for_code.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/shader_info_for_func.hpp>
#include <aiva2/shader_info_for_resource.hpp>
#include <aiva2/shader_info_for_resource_utils.hpp>
#include <aiva2/shader_info_for_struct.hpp>

namespace aiva2
{
	shader_info_for_code_t::shader_info_for_code_t(engine_t& engine, std::string const& text)
		: impl_type{ engine }
		, m_text{ text }
	{
		init_structs();
		init_resources();
		init_funcs();
	}

	shader_info_for_code_t::~shader_info_for_code_t()
	{
		shut_funcs();
		shut_resources();
		shut_structs();
	}

	auto shader_info_for_code_t::get_text() const->std::string const&
	{
		return m_text;
	}

	auto shader_info_for_code_t::has_text() const->bool
	{
		return !std::empty(m_text);
	}

	auto shader_info_for_code_t::get_struct_ref(size_t const index) const->shader_info_for_struct_t const&
	{
		assert_t::check_bool(index >= decltype(index){}, "(index) is not valid");
		assert_t::check_bool(index < std::size(m_structs), "(index) is not valid");

		auto const struct_ptr = get_struct_ptr(index);
		assert_t::check_bool(struct_ptr, "(struct_ptr) not valid");

		return (*struct_ptr);
	}

	auto shader_info_for_code_t::get_struct_ref(std::string_view const& name) const->shader_info_for_struct_t const&
	{
		assert_t::check_bool(!std::empty(name), "(name) is empty");

		auto const struct_ptr = get_struct_ptr(name);
		assert_t::check_bool(struct_ptr, "(struct_ptr) not valid");

		return (*struct_ptr);
	}

	auto shader_info_for_code_t::get_struct_ptr(size_t const index) const->std::shared_ptr<shader_info_for_struct_t const>
	{
		assert_t::check_bool(index >= decltype(index){}, "(index) is not valid");
		assert_t::check_bool(index < std::size(m_structs), "(index) is not valid");

		return m_structs[index];
	}

	auto shader_info_for_code_t::get_struct_ptr(std::string_view const& name) const->std::shared_ptr<shader_info_for_struct_t const>
	{
		assert_t::check_bool(!std::empty(name), "(name) is empty");

		auto const struct_itr = std::find_if(std::cbegin(m_structs), std::cend(m_structs), [&name](auto const& struct_ptr)
		{
			assert_t::check_bool(struct_ptr, "(struct_ptr) not valid");
			return (*struct_ptr).get_name() == name;
		});

		return struct_itr != std::cend(m_structs) ? (*struct_itr) : nullptr;
	}

	auto shader_info_for_code_t::num_struct() const->size_t
	{
		return std::size(m_structs);
	}

	void shader_info_for_code_t::init_structs()
	{
		m_structs = {};

		auto const regex = std::regex(R"(\bstruct\s+\w+\s*\{[\s\S]*?\};)", std::regex::icase | std::regex::optimize);

		for (auto i = std::sregex_iterator{ std::cbegin(m_text), std::cend(m_text), regex }; i != decltype(i){}; i++)
		{
			auto const& struct_match = (*i);
			assert_t::check_bool(struct_match.ready(), "(struct_match) is not valid");
			assert_t::check_bool(std::size(struct_match) == 1, "(struct_match) is not valid");

			auto const struct_string = struct_match.str();
			assert_t::check_bool(!std::empty(struct_string), "(struct_string) is not valid");

			auto const& struct_info = m_structs.emplace_back(std::make_shared<shader_info_for_struct_t>(get_engine(), struct_string));
			assert_t::check_bool(struct_info, "(struct_info) is not valid");
		}
	}

	void shader_info_for_code_t::shut_structs()
	{
		m_structs = {};
	}
	
	auto shader_info_for_code_t::get_resource_ref(size_t const index) const->shader_info_for_resource_t const&
	{
		assert_t::check_bool(index >= size_t{} && index < std::size(m_resources), "index is not valid");

		auto const& resource = m_resources[index];
		assert_t::check_bool(resource, "resource not valid");

		return (*resource);
	}

	auto shader_info_for_code_t::get_resource_ptr(size_t const index) const->std::shared_ptr<shader_info_for_resource_t const> const&
	{
		assert_t::check_bool(index >= size_t{} && index < std::size(m_resources), "index is not valid");

		auto const& resource = m_resources[index];
		assert_t::check_bool(resource, "resource not valid");

		return resource;
	}

	auto shader_info_for_code_t::num_resource() const->size_t
	{
		return std::size(m_resources);
	}

	void shader_info_for_code_t::init_resources()
	{
		m_resources = {};

		auto const regex_for_resources = std::regex(R"(\b\w+?\b *?(?:<[ \w]*?>) *?\b\w+?\b(?: *?\[[ \d]*?\] *?)*?: *?\bregister\b *?\([ \w]*?\) *?;)", std::regex::icase | std::regex::optimize);

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

		std::sort(std::begin(m_resources), std::end(m_resources));
	}

	void shader_info_for_code_t::shut_resources()
	{
		m_resources = {};
	}

	auto shader_info_for_code_t::get_func_ref(size_t const index) const->shader_info_for_func_t const&
	{
		auto const& func = get_func_ptr(index);
		assert_t::check_bool(func, "(func) not valid");

		return (*func);
	}

	auto shader_info_for_code_t::get_func_ref(std::string_view const& name) const->shader_info_for_func_t const&
	{
		auto const& func = get_func_ptr(name);
		assert_t::check_bool(func, "(func) not valid");

		return (*func);
	}

	auto shader_info_for_code_t::get_func_ptr(size_t const index) const->std::shared_ptr<shader_info_for_func_t const>
	{
		assert_t::check_bool(index >= decltype(index){}, "(index) is not valid");
		assert_t::check_bool(index < std::size(m_funcs), "(index) is not valid");

		return m_funcs[index];
	}

	auto shader_info_for_code_t::get_func_ptr(std::string_view const& name) const->std::shared_ptr<shader_info_for_func_t const>
	{
		assert_t::check_bool(!std::empty(name), "(name) is not valid");

		auto const func_itr = std::find_if(std::cbegin(m_funcs), std::cend(m_funcs), [&name](auto const& func)
		{
			assert_t::check_bool(func, "(func) not valid");
			return (*func).get_name() == name;
		});
		
		return func_itr != std::cend(m_funcs) ? (*func_itr) : nullptr;
	}

	auto shader_info_for_code_t::num_func() const->size_t
	{
		return std::size(m_funcs);
	}

	void shader_info_for_code_t::init_funcs()
	{
		m_funcs = {};

		auto const regex_for_func = std::regex(R"((?:^|\n)\s*(\b\w+\b\s+\b\w+\b\([\s\S]*?\)))", std::regex::icase | std::regex::optimize);

		for (auto i = std::sregex_iterator{ std::cbegin(m_text), std::cend(m_text), regex_for_func }; i != decltype(i){}; i++)
		{
			auto const& func_match = (*i);
			assert_t::check_bool(func_match.ready(), "(func_match) is not valid");
			assert_t::check_bool(std::size(func_match) == 2, "(func_match) is not valid");

			auto const func_string = func_match.str(1);
			assert_t::check_bool(!std::empty(func_string), "(func_string) is not valid");

			auto const& func_info = m_funcs.emplace_back(std::make_shared<shader_info_for_func_t>(get_engine(), func_string));
			assert_t::check_bool(func_info, "(func_info) is not valid");
		}
	}

	void shader_info_for_code_t::shut_funcs()
	{
		m_funcs = {};
	}

	auto shader_info_for_code_t::get_cbuffer_struct_ptr() const->std::shared_ptr<shader_info_for_struct_t const>
	{
		auto const resource_ptr = get_cbuffer_resource_ptr();
		if (!resource_ptr)
		{
			return {};
		}

		auto const& struct_name = (*resource_ptr).get_struct_name();
		if (std::empty(struct_name))
		{
			return {};
		}

		return get_struct_ptr(struct_name);
	}

	auto shader_info_for_code_t::get_cbuffer_struct_ref() const->shader_info_for_struct_t const&
	{
		auto const& struct_ptr = get_cbuffer_struct_ptr();
		assert_t::check_bool(struct_ptr, "(struct_ptr) not valid");

		return (*struct_ptr);
	}

	auto shader_info_for_code_t::get_cbuffer_resource_ptr() const->std::shared_ptr<shader_info_for_resource_t const>
	{
		for (auto i = size_t{}; i < num_resource(); i++)
		{
			auto const& resource_ptr = get_resource_ptr(i);
			assert_t::check_bool(resource_ptr, "(resource_ptr) not valid");

			if ((*resource_ptr).get_name() == C_BUFFER_RESOURCE_NAME)
			{
				return resource_ptr;
			}
		}

		return {};
	}

	auto shader_info_for_code_t::get_cbuffer_resource_ref() const->shader_info_for_resource_t const&
	{
		auto const& resource_ptr = get_cbuffer_resource_ptr();
		assert_t::check_bool(resource_ptr, "(resource) not valid");

		return (*resource_ptr);
	}
}
