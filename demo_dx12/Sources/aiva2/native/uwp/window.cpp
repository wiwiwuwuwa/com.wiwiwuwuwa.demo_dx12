#include <pch.h>
#include <aiva2/native/uwp/window.hpp>

namespace aiva2::native
{
	auto window_t::get_on_init() -> event_action_type<>&
	{
		return m_on_init;
	}

	auto window_t::get_on_tick() -> event_action_type<>&
	{
		return m_on_tick;
	}

	auto window_t::get_on_shut() -> event_action_type<>&
	{
		return m_on_shut;
	}

	auto window_t::get_core_window() const -> core_window_type const&
	{
		return m_core_window;
	}

	auto window_t::CreateView() -> IFrameworkView
	{
		return *this;
	}

	void window_t::Initialize(CoreApplicationView const&)
	{
		m_core_window = nullptr;
	}

	void window_t::Load(winrt::hstring const&)
	{

	}

	void window_t::Uninitialize()
	{
		m_core_window = nullptr;
	}

	void window_t::Run()
	{
		auto window = core_window_type::GetForCurrentThread();
		window.Activate();

		get_on_init().broadcast();

		while (true)
		{
			auto dispatcher = window.Dispatcher();
			dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			get_on_tick().broadcast();
		}

		get_on_shut().broadcast();
	}

	void window_t::SetWindow(core_window_type const& core_window)
	{
		m_core_window = core_window;
	}
}
