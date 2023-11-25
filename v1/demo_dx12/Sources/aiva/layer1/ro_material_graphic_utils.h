#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view_fwd.h>
#include <aiva/layer1/e_material_append_mode.h>
#include <aiva/layer1/i_shader_resource_fwd.h>
#include <aiva/layer1/res_view_desc.h>
#include <aiva/layer1/resource_view_heap_fwd.h>
#include <aiva/layer1/ro_material_graphic_fwd.h>

namespace aiva::layer1
{
	struct RoMaterialGraphicUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		RoMaterialGraphicUtils() = delete;

	// ----------------------------------------------------
	// Appending Utils

	public:
		static void Append(RoMaterialGraphicTypeShared const& dstMaterial, RoMaterialGraphicTypeShared const& srcMaterial, EMaterialAppendMode const appendMode);

		static void Append(RoMaterialGraphicTypeShared const& dstMaterial, ResourceViewHeapTypeShared const& srcHeap, EMaterialAppendMode const appendMode);

		static void Append(RoMaterialGraphicTypeShared const& dstMaterial, ResViewDescType const& srcDesc, EMaterialAppendMode const appendMode);

		static void Append(RoMaterialGraphicTypeShared const& dstMaterial, std::string const& dstName, ResViewDescType const& srcDesc, EMaterialAppendMode const appendMode);

		static void Append(RoMaterialGraphicTypeShared const& dstMaterial, std::string const& dstName, AGraphicResourceViewTypeShared const& srcView, EMaterialAppendMode const appendMode);

	// ----------------------------------------------------
	// Shader Resource Utils

	public:
		static void SetShaderResource(RoMaterialGraphicTypeShared const& dstMaterial, std::string const& srcVariableName, IShaderResourceTypeShared const& srcShaderResource);
	};
}
