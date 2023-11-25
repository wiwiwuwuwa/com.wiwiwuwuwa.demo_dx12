#include <pch.h>
#include <aiva/layer1_ext/gc_copy_buffer_to_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/graphic_hardware.h>
#include <aiva/layer1_ext/nr_buffer.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	GcCopyBufferToBuffer::GcCopyBufferToBuffer(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		OnExec().AttachListener(&ThisType::GraphicCommand_OnExec, this);
	}

	GcCopyBufferToBuffer::~GcCopyBufferToBuffer()
	{
		OnExec().RemoveListener(&ThisType::GraphicCommand_OnExec, this);
	}

	void GcCopyBufferToBuffer::GraphicCommand_OnExec()
	{
		ExecuteResourceBarrier();
		ExecuteCopyResource();
	}

	void GcCopyBufferToBuffer::ExecuteResourceBarrier()
	{
		auto resourceBarriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		{
			auto const& srcBuffer = mSrc;
			Asserts::CheckBool(srcBuffer, "Src buffer is not valid");

			auto const srcBarriers = srcBuffer->TransiteResourceBarriers({}, D3D12_RESOURCE_STATE_COPY_SOURCE);
			resourceBarriers.insert(std::cend(resourceBarriers), std::cbegin(srcBarriers), std::cend(srcBarriers));
		}

		{
			auto const& dstBuffer = mDst;
			Asserts::CheckBool(dstBuffer, "Dst buffer is not valid");

			auto const dstBarriers = dstBuffer->TransiteResourceBarriers({}, D3D12_RESOURCE_STATE_COPY_DEST);
			resourceBarriers.insert(std::cend(resourceBarriers), std::cbegin(dstBarriers), std::cend(dstBarriers));
		}

		if (std::empty(resourceBarriers))
		{
			return;
		}

		mEngine.GraphicHardwareExt().CommandList().ResourceBarrier(std::size(resourceBarriers), std::data(resourceBarriers));
	}

	void GcCopyBufferToBuffer::ExecuteCopyResource()
	{
		auto const& srcBuffer = mSrc;
		auto const& dstBuffer = mDst;

		Asserts::CheckBool(srcBuffer, "Src buffer is not valid");
		Asserts::CheckBool(dstBuffer, "Dst buffer is not valid");
		Asserts::CheckBool(srcBuffer->Size() == dstBuffer->Size(), "Buffers have the different sizes");

		auto const& srcResource = srcBuffer->Object();
		auto const& dstResource = dstBuffer->Object();

		Asserts::CheckBool(srcResource, "Src resource is not valid");
		Asserts::CheckBool(dstResource, "Dst resource is not valid");

		mEngine.GraphicHardwareExt().CommandList().CopyResource(dstResource.get(), srcResource.get());
	}
}
