#include <pch.h>
#include <aiva/layer1/shader_pipeline_descriptor.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::ShaderPipelineDescriptor::ShaderPipelineDescriptor(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
}

aiva::layer1::ShaderPipelineDescriptor::~ShaderPipelineDescriptor()
{
	TerminateCacheUpdater();
}

aiva::layer1::ShaderPipelineDescriptor::CacheUpdaterType& aiva::layer1::ShaderPipelineDescriptor::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::ShaderPipelineDescriptor::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::ShaderPipelineDescriptor::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}
