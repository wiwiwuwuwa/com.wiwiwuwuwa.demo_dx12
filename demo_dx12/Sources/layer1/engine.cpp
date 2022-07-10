#include <pch.h>
#include <layer1/engine.h>

#include <boost/bind.hpp>
#include <layer0/app.h>
#include <layer1/renderer.h>

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

void aiva::layer1::Engine::OnAppStart()
{
	mRenderer = std::make_unique<aiva::layer1::Renderer>(*this);
}

void aiva::layer1::Engine::OnAppUpdate()
{
	OnRender()();
	mTick++;
}

void aiva::layer1::Engine::OnAppFinish()
{
	mRenderer = {};
}

aiva::utils::EvAction& aiva::layer1::Engine::OnRender()
{
	return mOnRender;
}

uint64_t aiva::layer1::Engine::Tick() const
{
	return mTick;
}
