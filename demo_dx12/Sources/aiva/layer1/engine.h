#pragma once
#include <pch.h>

#include <memory>
#include <boost/core/noncopyable.hpp>
#include <winrt/base.h>
#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1/material_caching_system_fwd.h>
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
	struct ResourceSystem;
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

	// ----------------------------------------------------
	// App

	public:
		winrt::com_ptr<aiva::layer0::App> const& App() const;

	private:
		void InitializeApp();

		void TerminateApp();

	private:
		void OnAppStart();

		void OnAppUpdate();

		void OnAppFinish();

	private:
		winrt::com_ptr<aiva::layer0::App> mApp{};

	// ----------------------------------------------------
	// Events

	public:
		aiva::utils::EvAction& OnStart();

		aiva::utils::EvAction& OnUpdate();

		aiva::utils::EvAction& OnRender();

		aiva::utils::EvAction& OnFinish();

	private:
		aiva::utils::EvAction mOnStart{};

		aiva::utils::EvAction mOnUpdate{};

		aiva::utils::EvAction mOnRender{};

		aiva::utils::EvAction mOnFinish{};

	// ----------------------------------------------------
	// Loop

	public:
		void Run();

	// ----------------------------------------------------
	// Systems

	public:
		aiva::layer1::ResourceSystem& ResourceSystem() const;

		aiva::layer1::GraphicHardware& GraphicHardware() const;

		aiva::layer1::GraphicPipeline& GraphicPipeline() const;

		aiva::layer1::GraphicExecutor& GraphicExecutor() const;

		MaterialCachingSystemType& MaterialCachingSystem() const;

	private:
		void InitializeSystems();

		void TerminateSystems();

	private:
		std::unique_ptr<aiva::layer1::ResourceSystem> mResourceSystem{};

		std::unique_ptr<aiva::layer1::GraphicHardware> mGraphicHardware{};

		std::unique_ptr<aiva::layer1::GraphicPipeline> mGraphicPipeline{};

		std::unique_ptr<aiva::layer1::GraphicExecutor> mGraphicExecutor{};

		MaterialCachingSystemTypeShared mMaterialCachingSystem{};

	// ----------------------------------------------------
	// Tick

	public:
		uint64_t Tick() const;

	private:
		void RefreshTick();

	private:
		uint64_t mTick{};

	// ----------------------------------------------------
	// Time

	public:
		double DeltaTime() const;

	private:
		void InitializeTime();

		void TerminateTime();

	private:
		void RefreshTime();

	private:
		std::chrono::high_resolution_clock::time_point mDeltaTimeBegin{};

		std::chrono::high_resolution_clock::time_point mDeltaTimeEnd{};

		double mDeltaTime{};
	};
}
