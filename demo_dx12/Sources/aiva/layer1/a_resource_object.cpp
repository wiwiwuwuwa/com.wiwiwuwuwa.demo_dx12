#include <pch.h>
#include <aiva/layer1/a_resource_object.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	AResourceObject::AResourceObject(EngineType const& engine)
		: AObject{}, IObjectEngineable{ engine }
	{

	}

	AResourceObject::~AResourceObject()
	{

	}
}
