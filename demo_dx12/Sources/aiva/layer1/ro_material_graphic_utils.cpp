#include <pch.h>
#include <aiva/layer1/ro_material_graphic_utils.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/e_resource_view_type.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/layer1/res_view_desc_utils.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/resource_view_table.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	void RoMaterialGraphicUtils::Append(RoMaterialGraphicTypeShared const& dstMaterial, RoMaterialGraphicTypeShared const& srcMaterial, EMaterialAppendMode const appendMode)
	{
		Asserts::CheckBool(dstMaterial, "Dst material is not valid");
		Asserts::CheckBool(srcMaterial, "Src material is not valid");

		for (auto const& srcHeapPair : srcMaterial->ResourceDescriptor().ResourceTable().ResourceHeaps())
		{
			auto const& srcHeapType = srcHeapPair.first;
			auto const& srcHeapElem = srcHeapPair.second;
			Asserts::CheckBool(srcHeapElem, "Src heap elem is not valid");

			Append(dstMaterial, srcHeapElem, appendMode);
		}
	}

	void RoMaterialGraphicUtils::Append(RoMaterialGraphicTypeShared const& dstMaterial, ResourceViewHeapTypeShared const& srcHeap, EMaterialAppendMode const appendMode)
	{
		Asserts::CheckBool(dstMaterial, "Dst material is not valid");
		Asserts::CheckBool(srcHeap, "Src heap is not valid");

		for (auto const& srcViewPair : srcHeap->GetViews())
		{
			auto const& srcViewName = srcViewPair.first;
			auto const& srcViewElem = srcViewPair.second;
			Asserts::CheckBool(srcViewElem, "Src view elem is not valid");

			auto srcDesc = ResViewDescType{};
			srcDesc.Name = srcViewName;
			srcDesc.Heap = srcHeap;

			Append(dstMaterial, srcDesc, appendMode);
		}
	}

	void RoMaterialGraphicUtils::Append(RoMaterialGraphicTypeShared const& dstMaterial, ResViewDescType const& srcDesc, EMaterialAppendMode const appendMode)
	{
		Asserts::CheckBool(dstMaterial, "Dst material is not valid");
		Asserts::CheckBool(ResViewDescUtils::IsValid(srcDesc), "Src desc is not valid");

		Append(dstMaterial, srcDesc.Name, srcDesc, appendMode);
	}

	void RoMaterialGraphicUtils::Append(RoMaterialGraphicTypeShared const& dstMaterial, std::string const& dstName, ResViewDescType const& srcDesc, EMaterialAppendMode const appendMode)
	{
		Asserts::CheckBool(dstMaterial, "Dst material is not valid");
		Asserts::CheckBool(!std::empty(dstName), "Dst name is not valid");
		Asserts::CheckBool(ResViewDescUtils::IsValid(srcDesc), "Src desc is not valid");

		auto const srcView = ResViewDescUtils::GetView(srcDesc);
		Asserts::CheckBool(srcView, "Src view is not valid");

		Append(dstMaterial, dstName, srcView, appendMode);
	}

	void RoMaterialGraphicUtils::Append(RoMaterialGraphicTypeShared const& dstMaterial, std::string const& dstName, AGraphicResourceViewTypeShared const& srcView, EMaterialAppendMode const appendMode)
	{
		Asserts::CheckBool(dstMaterial, "Dst material is not valid");
		Asserts::CheckBool(!std::empty(dstName), "Dst name is not valid");
		Asserts::CheckBool(srcView, "Src view is not valid");

		auto const dstHeap = dstMaterial->ResourceDescriptor().ResourceTable().GetOrAddResourceHeap(ToDescriptorHeapType(srcView->ViewType()));
		Asserts::CheckBool(dstHeap, "Dst heap is not valid");

		auto const dstView = dstHeap->GetView(dstName);
		if (dstView && appendMode == EMaterialAppendMode::Soft) return;

		dstHeap->SetView(dstName, srcView);
	}
}
