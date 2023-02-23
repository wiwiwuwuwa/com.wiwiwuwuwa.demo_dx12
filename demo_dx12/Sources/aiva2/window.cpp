#include <pch.h>
#include <aiva2/window.hpp>

namespace aiva2
{
	auto window_t::get_core_window() const -> CoreWindow const&
	{
		return m_core_window;
	}

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
		auto const window = CoreWindow::GetForCurrentThread();
		window.Activate();

		get_on_init().invoke();

		while (true)
		{
			auto dispatcher = window.Dispatcher();
			dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			get_on_tick().invoke();
		}

		get_on_shut().invoke();
	}

	void window_t::SetWindow(CoreWindow const& core_window)
	{
		m_core_window = core_window;
	}
}
