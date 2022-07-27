#include <pch.h>
#include <aiva/layer1/shader_buffer.h>

#include <aiva/layer1/shader_consts.h>
#include <aiva/layer1/shader_struct.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::ShaderBuffer::ShaderBuffer()
{
	InitializeCacheUpdater();
}

aiva::layer1::ShaderBuffer::~ShaderBuffer()
{
	TerminateCacheUpdater();
}

aiva::layer1::ShaderBuffer::CacheUpdaterType& aiva::layer1::ShaderBuffer::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::ShaderBuffer::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::ShaderBuffer::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

std::shared_ptr<const aiva::layer1::ShaderStruct> const& aiva::layer1::ShaderBuffer::Struct() const
{
	return mStruct;
}

aiva::layer1::ShaderBuffer& aiva::layer1::ShaderBuffer::Struct(std::shared_ptr<const ShaderStruct> const& referenceStruct)
{
	mStruct = referenceStruct;
	mShaderStructs = {};
	CacheUpdater().MarkAsChanged();

	return *this;
}

aiva::layer1::ShaderBuffer& aiva::layer1::ShaderBuffer::Add(std::shared_ptr<const ShaderStruct> const& shaderStruct)
{
	aiva::utils::Asserts::CheckBool(mStruct);
	aiva::utils::Asserts::CheckBool(shaderStruct);
	aiva::utils::Asserts::CheckBool((*mStruct).HasSameFields(*shaderStruct));

	mShaderStructs.push_back(shaderStruct);
	CacheUpdater().MarkAsChanged();

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
