#include <pch.h>
#include <aiva/layer1_ext/ro_scene_gltf.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/logger.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	RoSceneGltf::RoSceneGltf(aiva::layer1::Engine& engine) : mEngine{ engine }
	{

	}

	RoSceneGltf::~RoSceneGltf()
	{

	}

	void RoSceneGltf::DeserealizeFromBinary(boost::span<const std::byte> const& resourceBin)
	{
		Asserts::CheckBool(!std::empty(resourceBin), "Resource bin is not valid");

		auto warnings = std::string{};
		auto errors = std::string{};

		auto loader = tinygltf::TinyGLTF{};
		auto const isLoaded = loader.LoadASCIIFromString(&mData, &errors, &warnings, reinterpret_cast<const char*>(std::data(resourceBin)), std::size(resourceBin), {});

		if (!std::empty(warnings))
		{
			Logger::LogToDebugConsole(warnings);
		}

		if (!std::empty(errors))
		{
			Logger::LogToDebugConsole(errors);
		}

		Asserts::CheckBool(isLoaded, "Failed to load gltf scene from binary data");
	}
}
