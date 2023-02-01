#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/core/object.hpp>

namespace aiva2::core
{
	struct engine_t final : public implements_t<engine_t, object_t>
	{
		// ------------------------------------------------

	public:
		engine_t();

		~engine_t() override;

		// ------------------------------------------------

	public:
		auto get_native() const -> native::engine_t&;

	private:
		void init_native();

		void shut_native();

	private:
		std::unique_ptr<native::engine_t> m_native{};

		// ------------------------------------------------
	};
}