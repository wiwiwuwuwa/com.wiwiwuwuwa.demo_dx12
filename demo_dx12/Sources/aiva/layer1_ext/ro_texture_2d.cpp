#include <pch.h>
#include <aiva/layer1_ext/ro_texture_2d.h>

#include <aiva/layer1/engine.h>

namespace aiva::layer1_ext
{
	RoTexture2D::RoTexture2D(aiva::layer1::Engine& engine) : mEngine{ engine }
	{

	}

	RoTexture2D::~RoTexture2D()
	{

	}

	void RoTexture2D::DeserealizeFromJson(nlohmann::json const& resourceJsn)
	{

	}
}
