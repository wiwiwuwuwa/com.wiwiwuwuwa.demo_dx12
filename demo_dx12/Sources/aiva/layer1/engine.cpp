#include <pch.h>
#include <aiva/layer1/engine.h>

#include <boost/bind.hpp>
#include <aiva/layer0/app.h>
#include <aiva/layer1/graphic_executor.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/graphic_pipeline.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/utils/asserts.h>

aiva::layer1::Engine::Engine()
{
	mApp = winrt::make_self<aiva::layer0::App>();
	winrt::check_bool(mApp);

	mApp->OnStart().connect(boost::bind(&aiva::layer1::Engine::OnAppStart, this));
	mApp->OnUpdate().connect(boost::bind(&aiva::layer1::Engine::OnAppUpdate, this));
	mApp->OnFinish().connect(boost::bind(&aiva::layer1::Engine::OnAppFinish, this));
}

aiva::layer1::Engine::~Engine()
{
	winrt::check_bool(mApp);

	mApp->OnFinish().disconnect(boost::bind(&aiva::layer1::Engine::OnAppFinish, this));
	mApp->OnUpdate().disconnect(boost::bind(&aiva::layer1::Engine::OnAppUpdate, this));
	mApp->OnStart().disconnect(boost::bind(&aiva::layer1::Engine::OnAppStart, this));

	mApp = {};
}

void aiva::layer1::Engine::Run()
{
	winrt::check_bool(mApp);
	CoreApplication::Run(*mApp);
}

winrt::com_ptr<aiva::layer0::App> const& aiva::layer1::Engine::App() const
{
	return mApp;
}

aiva::layer1::ResourceSystem& aiva::layer1::Engine::ResourceSystem() const
{
	aiva::utils::Asserts::CheckBool(mResourceSystem);
	return *mResourceSystem;
}

aiva::layer1::GraphicHardware& aiva::layer1::Engine::GraphicHardware() const
{
	aiva::utils::Asserts::CheckBool(mGraphicHardware);
	return *mGraphicHardware;
}

aiva::layer1::GraphicPipeline& aiva::layer1::Engine::GraphicPipeline() const
{
	aiva::utils::Asserts::CheckBool(mGraphicPipeline);
	return *mGraphicPipeline;
}

aiva::layer1::GraphicExecutor& aiva::layer1::Engine::GraphicExecutor() const
{
	aiva::utils::Asserts::CheckBool(mGraphicExecutor);
	return *mGraphicExecutor;
}

void aiva::layer1::Engine::OnAppStart()
{
	mResourceSystem = std::make_unique<aiva::layer1::ResourceSystem>(*this);
	mGraphicHardware = std::make_unique<aiva::layer1::GraphicHardware>(*this);
	mGraphicPipeline = std::make_unique<aiva::layer1::GraphicPipeline>(*this);
	mGraphicExecutor = std::make_unique<aiva::layer1::GraphicExecutor>(*this);
}

void aiva::layer1::Engine::OnAppUpdate()
{
	OnUpdate()();
	OnPrepareForRender()();
	OnRender()();
	mTick++;
}

void aiva::layer1::Engine::OnAppFinish()
{
	mGraphicExecutor = {};
	mGraphicPipeline = {};
	mGraphicHardware = {};
	mResourceSystem = {};
}

aiva::utils::EvAction& aiva::layer1::Engine::OnUpdate()
{
	return mOnUpdate;
}

aiva::utils::EvAction& aiva::layer1::Engine::OnPrepareForRender()
{
	return mOnPrepareForRender;
}

aiva::utils::EvAction& aiva::layer1::Engine::OnRender()
{
	return mOnRender;
}

uint64_t aiva::layer1::Engine::Tick() const
{
	return mTick;
}

void aiva::layer1::Engine::LogToDebugConsole(std::string const& message) const
{
	winrt::check_bool(mApp);
	mApp->LogToDebugConsole("aiva: " + message + "\n");
}
