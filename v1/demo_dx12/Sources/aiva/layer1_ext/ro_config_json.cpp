#include <pch.h>
#include <aiva/layer1_ext/ro_config_json.h>

#include <aiva/layer1/engine.h>

namespace aiva::layer1_ext
{
	RoConfigJson::RoConfigJson(aiva::layer1::Engine& engine) : mEngine{ engine }
	{

	}

	RoConfigJson::~RoConfigJson()
	{

	}

	void RoConfigJson::DeserealizeFromJson(nlohmann::json const& resourceJsn)
	{
		mData = resourceJsn;
	}
}
