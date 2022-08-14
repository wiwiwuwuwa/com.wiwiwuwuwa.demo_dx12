#include <pch.h>
#include <aiva/layer1/material_caching_system.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/hash_utils.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	MaterialCachingSystem::MaterialCachingSystem(EngineType const& engine) : AObject{}, IObjectEngineable{ engine }
	{
		InitializeMaterialCaching();
	}

	MaterialCachingSystem::~MaterialCachingSystem()
	{
		TerminateMaterialsCaching();
	}

	RoMaterialGraphicTypeShared MaterialCachingSystem::GetCachedMaterial(RoMaterialGraphicTypeShared const& sharedMaterial, MatKeyType const uniqueID)
	{
		Asserts::CheckBool(sharedMaterial, "Shared material is not valid");

		auto const hash = HashUtils::Hash(sharedMaterial, uniqueID);

		auto const existingIter = mCachedMaterials.find(hash);
		if (existingIter != std::end(mCachedMaterials))
		{
			auto const& existingMaterial = existingIter->second.Material;
			Asserts::CheckBool(existingMaterial, "Existing material is not valid");

			return existingMaterial;
		}

		auto const createdIter = mCachedMaterials.insert_or_assign(hash, MatValType{ sharedMaterial->Copy(), Engine().Tick() }).first;
		if (createdIter != std::end(mCachedMaterials))
		{
			auto const& createdMaterial = createdIter->second.Material;
			Asserts::CheckBool(createdMaterial, "Created material is not valid");

			return createdMaterial;
		}

		Asserts::CheckBool("Failed to get cached material");
		return {};
	}

	void MaterialCachingSystem::InitializeMaterialCaching()
	{
		// TODO: Fix
		const_cast<EngineType&>(Engine()).OnUpdate().connect(boost::bind(&MaterialCachingSystem::RefreshMaterialsCaching, this));
	}

	void MaterialCachingSystem::TerminateMaterialsCaching()
	{
		// TODO: Fix
		const_cast<EngineType&>(Engine()).OnUpdate().disconnect(boost::bind(&MaterialCachingSystem::RefreshMaterialsCaching, this));
	}

	void MaterialCachingSystem::RefreshMaterialsCaching()
	{
		auto materialsToRemove = std::vector<MatKeyType>{};

		for (auto const& pair : mCachedMaterials)
		{
			auto const& uniqueID = pair.first;
			auto const& cachingEntry = pair.second;

			if (Engine().Tick() - cachingEntry.Tick >= 100)
			{
				materialsToRemove.emplace_back(uniqueID);
			}
		}

		for (auto const& uniqueID : materialsToRemove)
		{
			mCachedMaterials.erase(uniqueID);
		}
	}
}
