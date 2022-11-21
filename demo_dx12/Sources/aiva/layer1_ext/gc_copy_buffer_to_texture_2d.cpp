#include <pch.h>
#include <aiva/layer1_ext/gc_copy_buffer_to_texture_2d.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/graphic_hardware.h>
#include <aiva/layer1_ext/nr_buffer.h>
#include <aiva/layer1_ext/nr_texture_2d.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	GcCopyBufferToTexture2D::GcCopyBufferToTexture2D(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		OnExec().AttachListener(&ThisType::GraphicCommand_OnExec, this);
	}

	GcCopyBufferToTexture2D::~GcCopyBufferToTexture2D()
	{
		OnExec().RemoveListener(&ThisType::GraphicCommand_OnExec, this);
	}

	void GcCopyBufferToTexture2D::GraphicCommand_OnExec()
	{
		ExecuteResourceBarrier();
		ExecuteCopyResource();
	}

	void GcCopyBufferToTexture2D::ExecuteResourceBarrier()
	{
		auto resourceBarriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		{
			auto const& srcObj = mSrcObj;
			Asserts::CheckBool(srcObj, "Src obj is not valid");

			auto const srcBar = srcObj->TransiteResourceBarriers({}, D3D12_RESOURCE_STATE_COPY_SOURCE);
			resourceBarriers.insert(std::cend(resourceBarriers), std::cbegin(srcBar), std::cend(srcBar));
		}

		{
			auto const& dstObj = mDstObj;
			Asserts::CheckBool(dstObj, "Dst obj is not valid");

			auto const dstBar = dstObj->TransiteResourceBarriers({}, D3D12_RESOURCE_STATE_COPY_DEST);
			resourceBarriers.insert(std::cend(resourceBarriers), std::cbegin(dstBar), std::cend(dstBar));
		}

		if (std::empty(resourceBarriers))
		{
			return;
		}

		mEngine.GraphicHardwareExt().CommandList().ResourceBarrier(std::size(resourceBarriers), std::data(resourceBarriers));
	}

	void GcCopyBufferToTexture2D::ExecuteCopyResource()
	{
		auto const& srcObj = mSrcObj;
		auto const& dstObj = mDstObj;

		Asserts::CheckBool(srcObj, "Src obj is not valid");
		Asserts::CheckBool(dstObj, "Dst obj is not valid");

		auto const& srcRes = srcObj->Object();
		auto const& dstRes = dstObj->Object();

		Asserts::CheckBool(srcRes, "Src resource is not valid");
		Asserts::CheckBool(dstRes, "Dst resource is not valid");

		auto const srcDsc = srcRes->GetDesc();
		auto const dstDsc = dstRes->GetDesc();

		auto srcLoc = D3D12_TEXTURE_COPY_LOCATION{};
		srcLoc.pResource = srcRes.get();
		srcLoc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		srcLoc.PlacedFootprint.Offset = {};
		srcLoc.PlacedFootprint.Footprint.Format = dstDsc.Format;
		srcLoc.PlacedFootprint.Footprint.Width = dstDsc.Width;
		srcLoc.PlacedFootprint.Footprint.Height = dstDsc.Height;
		srcLoc.PlacedFootprint.Footprint.Depth = dstDsc.DepthOrArraySize;
		srcLoc.PlacedFootprint.Footprint.RowPitch = srcDsc.Width / dstDsc.Height;

		auto dstLoc = D3D12_TEXTURE_COPY_LOCATION{};
		dstLoc.pResource = dstRes.get();
		dstLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dstLoc.SubresourceIndex = {};

		mEngine.GraphicHardwareExt().CommandList().CopyTextureRegion(&dstLoc, {}, {}, {}, &srcLoc, {});
	}
}
