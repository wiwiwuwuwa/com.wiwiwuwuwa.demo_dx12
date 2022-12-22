#include <pch.h>
#include <aiva2/engine/engine.h>

#include <aiva2/engine/asserts.h>
#include <aiva2/engine/object_utils.h>
#include <aiva2/engine/time_system.h>
#include <aiva2/engine/window_system.h>

namespace aiva2::engine
{
	Engine::Engine()
	{
		
	}

	Engine::~Engine()
	{
		
	}

	void Engine::Run()
	{
		WindowSystem().RunWindow();
	}

	void Engine::Init()
	{
		IObject::Init();
		InitSystems();
	}

	void Engine::Shut()
	{
		ShutSystems();
		IObject::Shut();
	}

	void Engine::InitSystems()
	{
		InitWindowSystem();
		InitTimeSystem();
		InitGraphicHardware();
	}

	void Engine::ShutSystems()
	{
		ShutGraphicHardware();
		ShutTimeSystem();
		ShutWindowSystem();
	}

	auto Engine::TimeSystem() const -> engine::TimeSystem&
	{
		Asserts::IsTrue(mTimeSystem, "Time system is not valid");
		return *mTimeSystem;
	}

	void Engine::InitTimeSystem()
	{
		Asserts::IsTrue(mTimeSystem, "Time system is not valid");
		mTimeSystem = NewObject<decltype(mTimeSystem)::element_type>(*this);
	}

	void Engine::ShutTimeSystem()
	{
		Asserts::IsTrue(mTimeSystem, "Time system is not valid");
		mTimeSystem = {};
	}
}
