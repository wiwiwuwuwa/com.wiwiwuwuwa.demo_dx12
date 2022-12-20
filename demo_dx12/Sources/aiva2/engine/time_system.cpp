#include <pch.h>
#include <aiva2/engine/time_system.h>

#include <aiva2/engine/engine.h>
#include <aiva2/engine/time_system.h>
#include <aiva2/engine/window_system.h>

namespace aiva2::engine
{
	TimeSystem::TimeSystem(engine::Engine& engine) : mEngine{ engine }
	{
		InitTime();
	}

	TimeSystem::~TimeSystem()
	{
		ShutTime();
	}

	void TimeSystem::InitTime()
	{
		mEngine.WindowSystem().OnWindowInit().AttachListener(&ThisType::Time_When_WindowSystem_OnWindowInit, this);
		mEngine.WindowSystem().OnWindowTick().AttachListener(&ThisType::Time_When_WindowSystem_OnWindowTick, this);
		mEngine.WindowSystem().OnWindowShut().AttachListener(&ThisType::Time_When_WindowSystem_OnWindowShut, this);
	}

	void TimeSystem::ShutTime()
	{
		mEngine.WindowSystem().OnWindowShut().RemoveListener(&ThisType::Time_When_WindowSystem_OnWindowShut, this);
		mEngine.WindowSystem().OnWindowTick().RemoveListener(&ThisType::Time_When_WindowSystem_OnWindowTick, this);
		mEngine.WindowSystem().OnWindowInit().RemoveListener(&ThisType::Time_When_WindowSystem_OnWindowInit, this);
	}

	void TimeSystem::Time_When_WindowSystem_OnWindowInit()
	{
		OnTimeInit().Broadcast();
	}

	void TimeSystem::Time_When_WindowSystem_OnWindowTick()
	{
		OnTimeTick().Broadcast();
		OnTimeDraw().Broadcast();
	}

	void TimeSystem::Time_When_WindowSystem_OnWindowShut()
	{
		OnTimeShut().Broadcast();
	}
}
