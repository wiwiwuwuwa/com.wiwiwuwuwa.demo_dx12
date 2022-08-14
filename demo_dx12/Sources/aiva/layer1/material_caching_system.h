#pragma once
#include <pch.h>

#include <aiva/layer1/i_object_engineable.h>
#include <aiva/layer1/material_caching_entry.h>
#include <aiva/layer1/ro_material_graphic_fwd.h>
#include <aiva/utils/a_object.h>

namespace aiva::layer1
{
	struct MaterialCachingSystem final : public aiva::utils::AObject, public IObjectEngineable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		MaterialCachingSystem(EngineType const& engine);

	public:
		~MaterialCachingSystem() override;

	// ----------------------------------------------------
	// Materials Caching

	public:
		using MatKeyType = std::size_t;

		using MatValType = MaterialCachingEntry;

		using MatMapType = std::unordered_map<MatKeyType, MatValType>;

	public:
		RoMaterialGraphicTypeShared GetCachedMaterial(RoMaterialGraphicTypeShared const& sharedMaterial, MatKeyType const uniqueID);

	private:
		void InitializeMaterialCaching();

		void TerminateMaterialsCaching();

	private:
		void RefreshMaterialsCaching();

	private:
		MatMapType mCachedMaterials{};
	};
}
