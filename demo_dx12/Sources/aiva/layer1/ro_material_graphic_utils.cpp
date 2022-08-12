#include <pch.h>
#include <aiva/layer1/ro_material_graphic_utils.h>

#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/resource_view_table.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	RoMaterialGraphicTypeShared RoMaterialGraphicUtils::Combine(RoMaterialGraphicTypeShared const& mainMaterial, RoMaterialGraphicTypeShared const& additionalMaterial)
	{
		Asserts::CheckBool(mainMaterial, "Main material is not valid");
		Asserts::CheckBool(additionalMaterial, "Additional material is not valid");

		auto const combinedMaterial = mainMaterial->Copy();
		Asserts::CheckBool(combinedMaterial);

		for (auto const& heapPair : additionalMaterial->ResourceDescriptor().ResourceTable().ResourceHeaps())
		{
			auto const& additionalHeap = heapPair.second;
			Asserts::CheckBool(additionalHeap, "Additional heap is not valid");

			auto const& combinedHeap = combinedMaterial->ResourceDescriptor().ResourceTable().GetOrAddResourceHeap(additionalHeap->HeapType());
			Asserts::CheckBool(combinedHeap, "Combined heap is not valid");

			for (auto const& viewPair : additionalHeap->GetViews())
			{
				auto const& additionalView = viewPair.second;
				Asserts::CheckBool(additionalView, "Additional view is not valid");

				combinedHeap->SetView(viewPair.first, additionalView);
			}
		}

		return combinedMaterial;
	}
}
