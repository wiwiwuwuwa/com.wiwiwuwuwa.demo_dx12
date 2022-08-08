#include <pch.h>
#include <aiva/layer1/i_object_engineable.h>

#include <aiva/layer1/engine.h>

aiva::layer1::IObjectEngineable::IObjectEngineable(EngineType const& engine) : mEngine{ engine }
{

}

aiva::layer1::IObjectEngineable::~IObjectEngineable()
{

}

aiva::layer1::IObjectEngineable::EngineType const& aiva::layer1::IObjectEngineable::Engine() const
{
	return mEngine;
}
