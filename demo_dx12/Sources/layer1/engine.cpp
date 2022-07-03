#include <pch.h>
#include <layer1/engine.h>

#include <boost/bind.hpp>
#include <layer0/app.h>
#include <layer1/renderer.h>

aiva::layer1::Engine::Engine()
{
	mApp = winrt::make_self<aiva::layer0::App>();
	winrt::check_bool(mApp);

	mApp->OnStart().connect(boost::bind(&aiva::layer1::Engine::OnStart, this));
	mApp->OnUpdate().connect(boost::bind(&aiva::layer1::Engine::OnUpdate, this));
	mApp->OnFinish().connect(boost::bind(&aiva::layer1::Engine::OnFinish, this));

	CoreApplication::Run(*mApp);

	mApp->OnFinish().disconnect(boost::bind(&aiva::layer1::Engine::OnFinish, this));
	mApp->OnUpdate().disconnect(boost::bind(&aiva::layer1::Engine::OnUpdate, this));
	mApp->OnStart().disconnect(boost::bind(&aiva::layer1::Engine::OnStart, this));

	mApp = {};
}

aiva::layer1::Engine::~Engine()
{

}

winrt::com_ptr<aiva::layer0::App> const& aiva::layer1::Engine::App() const
{
	return mApp;
}

void aiva::layer1::Engine::OnStart()
{
	mRenderer = std::make_unique<aiva::layer1::Renderer>(*this);
}

void aiva::layer1::Engine::OnUpdate()
{

}

void aiva::layer1::Engine::OnFinish()
{
	mRenderer = {};
}
