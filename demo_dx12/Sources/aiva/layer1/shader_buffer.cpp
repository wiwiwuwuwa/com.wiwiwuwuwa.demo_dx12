#include <pch.h>
#include <aiva/layer1/shader_buffer.h>

#include <aiva/layer1/shader_consts.h>
#include <aiva/layer1/shader_struct.h>
#include <aiva/utils/asserts.h>

aiva::layer1::ShaderBuffer::ShaderBuffer()
	: aiva::utils::AObject{}, aiva::utils::IObjectChangeable{}
{

}

aiva::layer1::ShaderBuffer::~ShaderBuffer()
{

}

std::shared_ptr<const aiva::layer1::ShaderStruct> const& aiva::layer1::ShaderBuffer::Struct() const
{
	return mStruct;
}

aiva::layer1::ShaderBuffer& aiva::layer1::ShaderBuffer::Struct(std::shared_ptr<const ShaderStruct> const& referenceStruct)
{
	mStruct = referenceStruct;
	mShaderStructs = {};

	OnChanged()();
	return *this;
}

aiva::layer1::ShaderBuffer& aiva::layer1::ShaderBuffer::Add(std::shared_ptr<const ShaderStruct> const& shaderStruct)
{
	aiva::utils::Asserts::CheckBool(mStruct);
	aiva::utils::Asserts::CheckBool(shaderStruct);
	aiva::utils::Asserts::CheckBool((*mStruct).HasSameFields(*shaderStruct));

	mShaderStructs.push_back(shaderStruct);

	OnChanged()();
	return *this;
}

std::shared_ptr<const aiva::layer1::ShaderStruct> const& aiva::layer1::ShaderBuffer::Get(std::size_t index) const
{
	return mShaderStructs.at(index);
}

std::size_t aiva::layer1::ShaderBuffer::Num() const
{
	return mShaderStructs.size();
}

std::size_t aiva::layer1::ShaderBuffer::ByteStride() const
{
	aiva::utils::Asserts::CheckBool(mStruct);
	return mStruct->SerializeToBinary().size();
}

std::vector<std::byte> aiva::layer1::ShaderBuffer::SerializeToBinary() const
{
	auto totalBytes = std::vector<std::byte>{};

	for (auto const& shaderStruct : mShaderStructs)
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
