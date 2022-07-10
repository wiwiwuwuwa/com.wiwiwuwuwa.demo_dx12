#pragma once
#include <pch.h>

#include <memory>
#include <boost/core/noncopyable.hpp>
#include <winrt/base.h>
#include <utils/ev_action.h>

namespace aiva::layer0
{
	struct App;
}

namespace aiva::layer1
{
	struct Renderer;
}

namespace aiva::layer1
{
	struct Engine final : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Main

	public:
		Engine();

		~Engine();

		void Run();

		winrt::com_ptr<aiva::layer0::App> const& App() const;

	private:
		void OnAppStart();

		void OnAppUpdate();

		void OnAppFinish();	

		winrt::com_ptr<aiva::layer0::App> mApp{};

		std::unique_ptr<aiva::layer1::Renderer> mRenderer{};

	// ----------------------------------------------------
	// Events

	public:
		aiva::utils::EvAction& OnRender();

	private:
		aiva::utils::EvAction mOnRender{};

	// ----------------------------------------------------
	// Time

	public:
		uint64_t Tick() const;

	private:
		uint64_t mTick{};
	};
}
