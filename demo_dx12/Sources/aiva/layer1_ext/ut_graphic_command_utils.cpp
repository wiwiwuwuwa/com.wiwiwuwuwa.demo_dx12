#include <pch.h>
#include <aiva/layer1_ext/ut_graphic_command_utils.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/gc_copy_buffer_to_buffer.h>
#include <aiva/layer1_ext/gc_copy_buffer_to_texture_2d.h>
#include <aiva/layer1_ext/graphic_executor.h>
#include <aiva/layer1_ext/nr_buffer.h>
#include <aiva/layer1_ext/nr_texture_2d.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	void UtGraphicCommandUtils::CopyDataToBuffer(boost::span<const std::byte> const& srcData, std::shared_ptr<NrBuffer> const& dstBuffer, aiva::layer1::Engine& engine)
	{
		Asserts::CheckBool(!std::empty(srcData), "Src data is not valid");
		Asserts::CheckBool(dstBuffer, "Dst buffer is not valid");
		Asserts::CheckBool(std::size(srcData) == dstBuffer->Size(), "Buffers has different sizes");

		auto const srcBuffer = NewObject<NrBuffer>(engine);
		Asserts::CheckBool(srcBuffer, "Src buffer is not valid");

		srcBuffer->MemoryType(aiva::layer1::EBufferMemoryType::CpuToGpu);
		srcBuffer->Size(std::size(srcData));
		srcBuffer->WriteBinaryData(srcData);

		CopyBufferToBuffer(srcBuffer, dstBuffer, engine);
	}

	void UtGraphicCommandUtils::CopyBufferToBuffer(std::shared_ptr<NrBuffer> const& srcBuffer, std::shared_ptr<NrBuffer> const& dstBuffer, aiva::layer1::Engine& engine)
	{
		Asserts::CheckBool(srcBuffer, "Src buffer is not valid");
		Asserts::CheckBool(dstBuffer, "Dst buffer is not valid");
		Asserts::CheckBool(srcBuffer->Size() == dstBuffer->Size(), "Buffers has different sizes");

		auto const command = NewObject<GcCopyBufferToBuffer>(engine);
		Asserts::CheckBool(command, "Command is not valid");
		command->Src(srcBuffer);
		command->Dst(dstBuffer);

		engine.GraphicExecutorExt().Execute(command);
	}

	void UtGraphicCommandUtils::CopyDataToTexture2D(boost::span<const std::byte> const& srcBin, std::shared_ptr<NrTexture2D> const& dstObj, aiva::layer1::Engine& engine)
	{
		Asserts::CheckBool(!std::empty(srcBin), "Src bin is not valid");
		Asserts::CheckBool(dstObj, "Dst obj is not valid");

		auto const srcObj = NewObject<NrBuffer>(engine);
		Asserts::CheckBool(srcObj, "Src obj is not valid");

		srcObj->MemoryType(aiva::layer1::EBufferMemoryType::CpuToGpu);
		srcObj->Size(std::size(srcBin));
		srcObj->WriteBinaryData(srcBin);

		CopyBufferToTexture2D(srcObj, dstObj, engine);
	}

	void UtGraphicCommandUtils::CopyBufferToTexture2D(std::shared_ptr<NrBuffer> const& srcObj, std::shared_ptr<NrTexture2D> const& dstObj, aiva::layer1::Engine& engine)
	{
		Asserts::CheckBool(srcObj, "Src obj is not valid");
		Asserts::CheckBool(dstObj, "Dst obj is not valid");

		auto const command = NewObject<GcCopyBufferToTexture2D>(engine);
		Asserts::CheckBool(command, "Command is not valid");

		command->SrcObj(srcObj);
		command->DstObj(dstObj);

		engine.GraphicExecutorExt().Execute(command);
	}
}
