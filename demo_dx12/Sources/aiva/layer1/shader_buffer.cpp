#include <pch.h>
#include <aiva/layer1/shader_buffer.h>

#include <aiva/layer1/shader_consts.h>
#include <aiva/layer1/shader_struct.h>
#include <aiva/utils/asserts.h>

aiva::layer1::ShaderBuffer::ShaderBuffer(std::shared_ptr<ShaderStruct> const& referenceStruct) : mReferenceStruct{ referenceStruct }
{
	aiva::utils::Asserts::CheckBool(referenceStruct);
}

aiva::layer1::ShaderBuffer::~ShaderBuffer()
{

}

std::vector<std::byte> aiva::layer1::ShaderBuffer::SerializeToBinary() const
{
	std::vector<std::byte> totalBytes{};

	for (const auto& shaderStruct : mShaderStructs)
	{
		aiva::utils::Asserts::CheckBool(shaderStruct);

		auto const structBytes = shaderStruct->SerializeToBinary();
		aiva::utils::Asserts::CheckBool(!structBytes.empty());

		auto const pointer = totalBytes.size();

		totalBytes.resize(totalBytes.size() + structBytes.size());
		aiva::utils::Asserts::CheckBool(memcpy_s(totalBytes.data() + pointer, structBytes.size(), structBytes.data(), structBytes.size()) == 0);
	}

	{
		auto const totalSize = aiva::utils::MathUtils::Align(totalBytes.size(), ShaderConsts::HEAP_SIZE);
		totalBytes.resize(totalSize);
	}

	return totalBytes;
}

std::size_t aiva::layer1::ShaderBuffer::ByteStride() const
{
	aiva::utils::Asserts::CheckBool(mReferenceStruct);
	return mReferenceStruct->SerializeToBinary().size();
}

void aiva::layer1::ShaderBuffer::Add(std::shared_ptr<ShaderStruct> const& shaderStruct)
{
	aiva::utils::Asserts::CheckBool(mReferenceStruct);
	aiva::utils::Asserts::CheckBool(shaderStruct);
	aiva::utils::Asserts::CheckBool((*mReferenceStruct).HasSameFields(*shaderStruct));

	mShaderStructs.push_back(shaderStruct);
}

std::shared_ptr<const aiva::layer1::ShaderStruct> const& aiva::layer1::ShaderBuffer::Get(std::size_t index) const
{
	return mShaderStructs.at(index);
}

std::size_t aiva::layer1::ShaderBuffer::Num() const
{
	return mShaderStructs.size();
}
