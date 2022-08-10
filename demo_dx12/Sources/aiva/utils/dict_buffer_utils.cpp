#include <pch.h>
#include <aiva/utils/dict_buffer_utils.h>

#include <aiva/utils/asserts.h>

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
	Asserts::CheckBool(false, "TODO: Impl - DictBufferUtils::SerializeToBinary(DictBufferPointerType)");
	return {};
}

std::vector<std::byte> aiva::utils::DictBufferUtils::SerializeToBinary(DictBufferPointerType const& dictBuffer, MetaBufferPointerType const& metaBuffer)
{
	Asserts::CheckBool(false, "TODO: Impl - DictBufferUtils::SerializeToBinary(DictBufferPointerType, MetaBufferPointerType)");
	return {};
}

std::vector<std::byte> aiva::utils::DictBufferUtils::SerializeToBinary(DictBufferPointerType const& dictBuffer, LayoutBufferPointerType const& layoutBuffer)
{
	Asserts::CheckBool(false, "TODO: Impl - DictBufferUtils::SerializeToBinary(DictBufferPointerType, LayoutBufferPointerType)");
	return {};
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
