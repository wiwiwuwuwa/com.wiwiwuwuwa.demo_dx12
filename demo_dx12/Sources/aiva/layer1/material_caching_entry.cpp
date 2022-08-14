#include <pch.h>
#include <aiva/layer1/material_caching_entry.h>

namespace aiva::layer1
{
	MaterialCachingEntry::MaterialCachingEntry()
	{

	}

	MaterialCachingEntry::MaterialCachingEntry(RoMaterialGraphicTypeShared const& material, std::uint64_t const tick) : Material{ material }, Tick{ tick }
	{

	}
}
