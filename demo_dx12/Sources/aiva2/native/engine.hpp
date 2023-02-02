#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/core/object.hpp>

namespace aiva2::native
{
	struct engine_t final : public core::implements_t<engine_t, core::object_t>
	{
		// ------------------------------------------------
		// engine

	public:
		engine_t();

		~engine_t() override;

		// ------------------------------------------------

	public:
		void run() const;

		// ------------------------------------------------
		// window system

	public:
		auto get_window_system() const -> window_system_t&;

	private:
		void init_window_system();

		void shut_window_system();

	private:
		std::unique_ptr<window_system_t> m_window_system{};
	};
}
