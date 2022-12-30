#include <pch.h>
#include <aiva2/engine/engine.h>

#include <aiva2/engine/asserts.h>
#include <aiva2/engine/object_utils.h>
#include <aiva2/native/engine.h>

namespace aiva2::engine
{
	Engine::Engine()
	{
		InitNative();
	}

	Engine::~Engine()
	{
		ShutNative();
	}

	void Engine::Run() const
	{
		Native().Run();
	}

	auto Engine::Native() const -> native::Engine&
	{
		Asserts::IsTrue(mNative, "Native is not valid");
		return *mNative;
	}

	void Engine::InitNative()
	{
		mNative = NewObject<decltype(mNative)::element_type>();
		Asserts::IsTrue(mNative, "Native is not valid");
	}

	void Engine::ShutNative()
	{
		Asserts::IsTrue(mNative, "Native is not valid");
		mNative = {};
	}
}
