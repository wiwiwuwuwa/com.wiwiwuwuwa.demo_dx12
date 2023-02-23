#pragma once
#include <aiva2/_core.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct window_t : winrt::implements<window_t, IFrameworkViewSource, IFrameworkView>, public implements_t<window_t>
	{
		// ------------------------------------------------

	public:
		auto get_core_window() const->CoreWindow const&;

		auto get_on_init() -> event_action_type<>&;

		auto get_on_tick() -> event_action_type<>&;

		auto get_on_shut() -> event_action_type<>&;
		
	private:
		CoreWindow m_core_window{ nullptr };
		
		event_action_type<> m_on_init{};

		event_action_type<> m_on_tick{};

		event_action_type<> m_on_shut{};
		
		// ------------------------------------------------
		// i_framework_view_source
		
	public:
		auto CreateView() -> IFrameworkView;

		// ------------------------------------------------
		// i_framework_view

	public:
		void Initialize(CoreApplicationView const&);
		
		void Load(winrt::hstring const&);

		void Uninitialize();

		void Run();

		void SetWindow(CoreWindow const& core_window);

		// ------------------------------------------------
	};
}
