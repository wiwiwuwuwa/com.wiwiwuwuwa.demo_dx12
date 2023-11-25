#include <pch.h>
#include <aiva/utils/dict_buffer_utils.h>

#include <aiva/utils/asserts.h>
#include <aiva/utils/dict_buffer.h>
#include <aiva/utils/dict_struct.h>
#include <aiva/utils/dict_struct_utils.h>
#include <aiva/utils/layout_buffer.h>
#include <aiva/utils/layout_buffer_utils.h>
#include <aiva/utils/layout_struct.h>
#include <aiva/utils/layout_struct_utils.h>
#include <aiva/utils/meta_buffer.h>
#include <aiva/utils/meta_buffer_utils.h>

aiva::utils::DictBufferUtils::DictBufferPointerType aiva::utils::DictBufferUtils::GenerateFrom(LayoutBufferPointerType const& layoutBuffer)
{
	Asserts::CheckBool(false, "TODO: Impl - DictBufferUtils::GenerateFrom(LayoutBufferPointerType)");
	return {};
}

aiva::utils::DictBufferUtils::DictBufferPointerType aiva::utils::DictBufferUtils::GenerateFrom(MetaBufferPointerType const& metaBuffer)
{
	Asserts::CheckBool(false, "TODO: Impl - DictBufferUtils::GenerateFrom(MetaBufferPointerType)");
	return {};
}

std::vector<std::byte> aiva::utils::DictBufferUtils::SerializeToBinary(DictBufferPointerType const& dictBuffer)
{
	Asserts::CheckBool(dictBuffer, "Dict buffer is not valid");

	auto const layoutBuffer = LayoutBufferUtils::GenerateFrom(dictBuffer);
	Asserts::CheckBool(layoutBuffer, "Layout buffer is not valid");

	auto const bufferBinary = SerializeToBinary(dictBuffer, layoutBuffer);
	Asserts::CheckBool(!std::empty(bufferBinary), "Biffer binary is empty");

	return bufferBinary;
}

std::vector<std::byte> aiva::utils::DictBufferUtils::SerializeToBinary(DictBufferPointerType const& dictBuffer, MetaBufferPointerType const& metaBuffer)
{
	Asserts::CheckBool(dictBuffer, "Dict buffer is not valid");

	auto const layoutBuffer = LayoutBufferUtils::GenerateFrom(metaBuffer);
	Asserts::CheckBool(layoutBuffer, "Layout buffer is not valid");

	auto const bufferBinary = SerializeToBinary(dictBuffer, layoutBuffer);
	Asserts::CheckBool(!std::empty(bufferBinary), "Buffer binary is not valid");

	return bufferBinary;
}

std::vector<std::byte> aiva::utils::DictBufferUtils::SerializeToBinary(DictBufferPointerType const& dictBuffer, LayoutBufferPointerType const& layoutBuffer)
{
	Asserts::CheckBool(dictBuffer, "Dict buffer is not valid");
	Asserts::CheckBool(layoutBuffer, "Layout buffer is not valid");

	Asserts::CheckBool(dictBuffer->Layout(), "Dict buffer layout is not valid");
	Asserts::CheckBool(layoutBuffer->Layout(), "Layout buffer layout is not valid");
	Asserts::CheckBool(layoutBuffer->Stride() > 0, "Layout buffer stride is not valid");
	Asserts::CheckBool(layoutBuffer->Offset() >= 0, "Layout buffer offset is not valid");
	Asserts::CheckBool(layoutBuffer->Size() > 0, "Layout buffer size is not valid");
	Asserts::CheckBool(layoutBuffer->Stride() * dictBuffer->Num() <= layoutBuffer->Size(), "Layout buffer size is too small to contain all strides");
	Asserts::CheckBool(layoutBuffer->Layout()->Offset() >= 0, "Layout buffer layout struct offset is not valid");
	Asserts::CheckBool(layoutBuffer->Layout()->Size() > 0, "Layout buffer layout struct size is not valid");
	Asserts::CheckBool(layoutBuffer->Stride() >= layoutBuffer->Layout()->Offset() + layoutBuffer->Layout()->Size(), "Layout buffer struct is too big for layout buffer stride");
	Asserts::CheckBool(LayoutStructUtils::IsMatchingLayout(layoutBuffer->Layout(), dictBuffer->Layout()), "Dict struct is not matching layout struct");

	auto bufferBinary = std::vector<std::byte>{ layoutBuffer->Size() };

	for (std::size_t i = {}; i < dictBuffer->Num(); i++)
	{
		auto const& dictStruct = dictBuffer->Get(i);
		Asserts::CheckBool(dictStruct, "Dict struct is not valid");

		auto const structBinary = DictStructUtils::SerializeToBinary(dictStruct, layoutBuffer->Layout());
		Asserts::CheckBool(!std::empty(structBinary), "Struct binary is empty");
		Asserts::CheckBool(layoutBuffer->Layout()->Size() == std::size(structBinary), "Struct binary has the different size than layout struct");

		auto const srcSpan = boost::span{ structBinary };
		auto const dstSpan = boost::span{ bufferBinary }
			.subspan(layoutBuffer->Stride() * i, layoutBuffer->Stride())
			.subspan(layoutBuffer->Layout()->Offset(), layoutBuffer->Layout()->Size());

		Asserts::CheckBool(memcpy_s(std::data(dstSpan), std::size(dstSpan), std::data(srcSpan), std::size(srcSpan)) == 0, "Failed to memcpy_s");
	}

	return bufferBinary;
}

aiva::utils::DictBufferUtils::DictBufferPointerType aiva::utils::DictBufferUtils::DeserealizeFromBinary(MetaBufferPointerType const& metaBuffer, boost::span<const std::byte> const& bufferBinary)
{
	Asserts::CheckBool(false, "TODO: Impl - DictBufferUtils::DeserealizeFromBinary(MetaBufferPointerType, boost::span<const std::byte>)");
	return {};
}

aiva::utils::DictBufferUtils::DictBufferPointerType aiva::utils::DictBufferUtils::DeserealizeFromBinary(LayoutBufferPointerType const& metaBuffer, boost::span<const std::byte> const& bufferBinary)
{
	Asserts::CheckBool(false, "TODO: Impl - DictBufferUtils::DeserealizeFromBinary(LayoutBufferPointerType, boost::span<const std::byte>)");
	return {};
}
