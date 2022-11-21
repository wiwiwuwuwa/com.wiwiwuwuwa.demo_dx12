#include <pch.h>
#include <aiva/layer1/engine.h>

#include <boost/bind.hpp>
#include <aiva/layer0/app.h>
#include <aiva/layer1/graphic_executor.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/graphic_pipeline.h>
#include <aiva/layer1/material_caching_system.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1_ext/graphic_executor.h>
#include <aiva/layer1_ext/graphic_hardware.h>
#include <aiva/layer1_ext/resource_system.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/object_utils.h>
#include <aiva/utils_ext/object_utils.h>

aiva::layer1::Engine::Engine()
{
	InitializeApp();
}

aiva::layer1::Engine::~Engine()
{
	TerminateApp();
}

winrt::com_ptr<aiva::layer0::App> const& aiva::layer1::Engine::App() const
{
	return mApp;
}

void aiva::layer1::Engine::InitializeApp()
{
	mApp = winrt::make_self<aiva::layer0::App>();
	winrt::check_bool(mApp);

	mApp->OnStart().connect(boost::bind(&aiva::layer1::Engine::OnAppStart, this));
	mApp->OnUpdate().connect(boost::bind(&aiva::layer1::Engine::OnAppUpdate, this));
	mApp->OnFinish().connect(boost::bind(&aiva::layer1::Engine::OnAppFinish, this));
}

void aiva::layer1::Engine::TerminateApp()
{
	winrt::check_bool(mApp);

	mApp->OnFinish().disconnect(boost::bind(&aiva::layer1::Engine::OnAppFinish, this));
	mApp->OnUpdate().disconnect(boost::bind(&aiva::layer1::Engine::OnAppUpdate, this));
	mApp->OnStart().disconnect(boost::bind(&aiva::layer1::Engine::OnAppStart, this));

	mApp = {};
}

void aiva::layer1::Engine::OnAppStart()
{
	InitializeSystems();
	InitializeTime();

	OnStart()();
}

void aiva::layer1::Engine::OnAppUpdate()
{
	RefreshTime();

	OnUpdate()();
	OnRender()();

	RefreshTick();
}

void aiva::layer1::Engine::OnAppFinish()
{
	OnFinish()();

	TerminateTime();
	TerminateSystems();
}

aiva::utils::EvAction& aiva::layer1::Engine::OnStart()
{
	return mOnStart;
}

aiva::utils::EvAction& aiva::layer1::Engine::OnUpdate()
{
	return mOnUpdate;
}

aiva::utils::EvAction& aiva::layer1::Engine::OnRender()
{
	return mOnRender;
}

aiva::utils::EvAction& aiva::layer1::Engine::OnFinish()
{
	return mOnFinish;
}

void aiva::layer1::Engine::Run()
{
	winrt::check_bool(mApp);
	CoreApplication::Run(*mApp);
}

aiva::layer1::ResourceSystem& aiva::layer1::Engine::ResourceSystem() const
{
	aiva::utils::Asserts::CheckBool(mResourceSystem);
	return *mResourceSystem;
}

aiva::layer1_ext::ResourceSystem& aiva::layer1::Engine::ResourceSystemExt() const
{
	aiva::utils::Asserts::CheckBool(mResourceSystemExt, "Resource system ext is not valid");
	return *mResourceSystemExt;
}

aiva::layer1::GraphicHardware& aiva::layer1::Engine::GraphicHardware() const
{
	aiva::utils::Asserts::CheckBool(mGraphicHardware);
	return *mGraphicHardware;
}

aiva::layer1_ext::GraphicHardware& aiva::layer1::Engine::GraphicHardwareExt() const
{
	aiva::utils::Asserts::CheckBool(mGraphicHardwareExt, "Graphic hardware ext is not valid");
	return *mGraphicHardwareExt;
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

aiva::layer1_ext::GraphicExecutor& aiva::layer1::Engine::GraphicExecutorExt() const
{
	aiva::utils::Asserts::CheckBool(mGraphicExecutorExt, "Graphic executor ext is not valid");
	return *mGraphicExecutorExt;
}

aiva::layer1::MaterialCachingSystemType& aiva::layer1::Engine::MaterialCachingSystem() const
{
	aiva::utils::Asserts::CheckBool(mMaterialCachingSystem, "Material caching system is not valid");
	return *mMaterialCachingSystem;
}

void aiva::layer1::Engine::InitializeSystems()
{
	mResourceSystem = std::make_unique<aiva::layer1::ResourceSystem>(*this);
	mResourceSystemExt = aiva::utils_ext::NewObject<decltype(mResourceSystemExt)::element_type>(*this);
	mGraphicHardware = std::make_unique<aiva::layer1::GraphicHardware>(*this);
	mGraphicHardwareExt = aiva::utils_ext::NewObject<decltype(mGraphicHardwareExt)::element_type>(*this);
	mGraphicPipeline = std::make_unique<aiva::layer1::GraphicPipeline>(*this);
	mGraphicExecutor = std::make_unique<aiva::layer1::GraphicExecutor>(*this);
	mGraphicExecutorExt = aiva::utils_ext::NewObject<decltype(mGraphicExecutorExt)::element_type>(*this);
	mMaterialCachingSystem = aiva::utils::NewObject<MaterialCachingSystemType>(*this);
}

void aiva::layer1::Engine::TerminateSystems()
{
	mMaterialCachingSystem = {};
	mGraphicExecutor = {};
	mGraphicPipeline = {};
	mGraphicHardwareExt = {};
	mGraphicHardware = {};
	mResourceSystemExt = {};
	mResourceSystem = {};
}

uint64_t aiva::layer1::Engine::Tick() const
{
	return mTick;
}

void aiva::layer1::Engine::RefreshTick()
{
	mTick++;
}

double aiva::layer1::Engine::DeltaTime() const
{
	return mDeltaTime;
}

void aiva::layer1::Engine::InitializeTime()
{
	mDeltaTimeBegin = std::chrono::high_resolution_clock::now();
	mDeltaTimeEnd = {};
}

void aiva::layer1::Engine::TerminateTime()
{
	mDeltaTimeEnd = {};
	mDeltaTimeBegin = {};
}

void aiva::layer1::Engine::RefreshTime()
{
	mDeltaTimeEnd = std::chrono::high_resolution_clock::now();
	mDeltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(mDeltaTimeEnd - mDeltaTimeBegin).count();
	mDeltaTimeBegin = std::chrono::high_resolution_clock::now();
}
