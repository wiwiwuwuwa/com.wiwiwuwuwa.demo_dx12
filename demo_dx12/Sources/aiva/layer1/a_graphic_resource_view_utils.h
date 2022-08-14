#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view_fwd.h>
#include <aiva/layer1/res_view_desc.h>

namespace aiva::layer1
{
	struct AGraphicResourceViewUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		AGraphicResourceViewUtils() = delete;

	// ----------------------------------------------------
	// Utils

	public:
		template <typename TDstViewType>
		static std::shared_ptr<TDstViewType> TransformView(ResViewDescType const& srcDesc);

		template <typename TDstViewType>
		static std::shared_ptr<TDstViewType> TransformView(AGraphicResourceViewTypeShared const& srcView);
	};
}

// --------------------------------------------------------

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/res_view_desc_utils.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/object_utils.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	template <typename TDstViewType>
	std::shared_ptr<TDstViewType> AGraphicResourceViewUtils::TransformView(ResViewDescType const& srcDesc)
	{
		Asserts::CheckBool(ResViewDescUtils::IsValid(srcDesc), "Src desc is not valid");

		auto const srcView = ResViewDescUtils::GetView(srcDesc);
		Asserts::CheckBool(srcView, "Src view is not valid");

		auto const dstView = Reinterpret<TDstViewType>(srcView);
		Asserts::CheckBool(dstView, "Dst view is not valid");

		return dstView;
	}

	template <typename TDstViewType>
	std::shared_ptr<TDstViewType> AGraphicResourceViewUtils::TransformView(AGraphicResourceViewTypeShared const& srcView)
	{
		Asserts::CheckBool(srcView, "Src view is not valid");

		auto const extView = std::dynamic_pointer_cast<TDstViewType>(srcView);
		if (extView) return extView;

		auto const midBuffer = srcView->GetOrAddInternalResource();
		Asserts::CheckBool(midBuffer, "Mid buffer is not valid");

		auto const dstView = NewObject<TDstViewType>(srcView->Engine());
		Asserts::CheckBool(dstView, "Dst view is not valid");

		dstView->SetInternalResource(midBuffer);
		rerturn dstView;
	}
}
