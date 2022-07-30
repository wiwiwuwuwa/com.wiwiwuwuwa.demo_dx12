#include <pch.h>
#include <aiva/layer1/ro_scene_gltf.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>

aiva::layer1::RoSceneGltf::RoSceneGltf(aiva::layer1::Engine const& engine) : mEngine{ engine }
{

}

aiva::layer1::RoSceneGltf::~RoSceneGltf()
{

}

void aiva::layer1::RoSceneGltf::DeserealizeFromBinary(std::vector<std::byte> const& binaryData)
{
	aiva::utils::Asserts::CheckBool(!binaryData.empty());
}
