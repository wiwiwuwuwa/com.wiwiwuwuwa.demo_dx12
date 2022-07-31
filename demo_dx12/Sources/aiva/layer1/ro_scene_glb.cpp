#include <pch.h>
#include <aiva/layer1/ro_scene_glb.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::RoSceneGlb::RoSceneGlb(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
}

aiva::layer1::RoSceneGlb::~RoSceneGlb()
{
	TerminateCacheUpdater();
}

void aiva::layer1::RoSceneGlb::DeserealizeFromBinary(std::vector<std::byte> const& binaryData)
{
	aiva::utils::Asserts::CheckBool(!binaryData.empty());

	auto loader = tinygltf::TinyGLTF{};

	auto model = tinygltf::Model{};
	auto errors = std::string{};
	auto warnings = std::string{};
	if (!loader.LoadBinaryFromMemory(&model, &errors, &warnings, reinterpret_cast<const std::uint8_t*>(binaryData.data()), binaryData.size()))
	{
		if (!errors.empty())
		{
			mEngine.LogToDebugConsole(errors);
		}

		if (!warnings.empty())
		{
			mEngine.LogToDebugConsole(warnings);
		}
	}

	mModel = model;
	CacheUpdater().MarkAsChanged();
}

aiva::layer1::RoSceneGlb::CacheUpdaterType& aiva::layer1::RoSceneGlb::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::RoSceneGlb::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::RoSceneGlb::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

tinygltf::Model const& aiva::layer1::RoSceneGlb::Model() const
{
	return mModel;
}
