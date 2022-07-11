#pragma once
#include <pch.h>

#include <memory>
#include <boost/core/noncopyable.hpp>
#include <winrt/base.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer0
{
	struct App;
}

namespace aiva::layer1
{
	struct GraphicExecutor;
	struct GraphicHardware;
	struct GraphicPipeline;
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

		aiva::layer1::GraphicHardware& GraphicHardware() const;

		aiva::layer1::GraphicPipeline& GraphicPipeline() const;

		aiva::layer1::GraphicExecutor& GraphicExecutor() const;

	private:
		void OnAppStart();

		void OnAppUpdate();

		void OnAppFinish();	

		winrt::com_ptr<aiva::layer0::App> mApp{};

		std::unique_ptr<aiva::layer1::GraphicHardware> mGraphicHardware{};

		std::unique_ptr<aiva::layer1::GraphicPipeline> mGraphicPipeline{};

		std::unique_ptr<aiva::layer1::GraphicExecutor> mGraphicExecutor{};

	// ----------------------------------------------------
	// Events

	public:
		aiva::utils::EvAction& OnUpdate();

		aiva::utils::EvAction& OnPrepareForRender();

		aiva::utils::EvAction& OnRender();

	private:
		aiva::utils::EvAction mOnUpdate{};

		aiva::utils::EvAction mOnPrepareForRender{};

		aiva::utils::EvAction mOnRender{};

	// ----------------------------------------------------
	// Time

	public:
		uint64_t Tick() const;

	private:
		uint64_t mTick{};
	};
}
