#include <pch.h>
#include <aiva2/core/engine.h>

#include <aiva2/core/asserts.h>
#include <aiva2/native/engine.h>

namespace aiva2::core
{
	engine_t::engine_t()
	{
		init_native();
	}

	engine_t::~engine_t()
	{
		shut_native();
	}

	auto engine_t::get_native() const -> native::engine_t&
	{
		asserts_t::check_true(m_native, "native engine is not valid");
		return *m_native;
	}

	void engine_t::init_native()
	{
		m_native = std::make_unique<native::engine_t>();
		asserts_t::check_true(m_native, "native engine is not valid");
	}

	void engine_t::shut_native()
	{
		asserts_t::check_true(m_native, "native engine is not valid");
		m_native = {};
	}
}
