#include <pch.h>
#include <aiva/layer1/shader_pipeline_descriptor.h>

#include <aiva/layer1/engine.h>

aiva::layer1::ShaderPipelineDescriptor::ShaderPipelineDescriptor(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
	InitializeInternalResources();
}

aiva::layer1::ShaderPipelineDescriptor::~ShaderPipelineDescriptor()
{
	TerminateInternalResources();
	TerminateCacheUpdater();
}

aiva::utils::TCacheRefresh<aiva::layer1::ShaderPipelineDescriptor::EDirtyFlags>& aiva::layer1::ShaderPipelineDescriptor::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::ShaderPipelineDescriptor::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<decltype(mCacheUpdater)::element_type>(EDirtyFlags::All);
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::ShaderPipelineDescriptor::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

void aiva::layer1::ShaderPipelineDescriptor::InitializeInternalResources()
{
	CacheUpdater().OnFlushCompleted().connect(boost::bind(&ShaderPipelineDescriptor::NotifyInternalResourcesUpdated, this));
}

void aiva::layer1::ShaderPipelineDescriptor::TerminateInternalResources()
{
	CacheUpdater().OnFlushCompleted().disconnect(boost::bind(&ShaderPipelineDescriptor::NotifyInternalResourcesUpdated, this));
}

void aiva::layer1::ShaderPipelineDescriptor::NotifyInternalResourcesUpdated()
{
	OnInternalResourceUpdated()();
}

aiva::utils::EvAction& aiva::layer1::ShaderPipelineDescriptor::OnInternalResourceUpdated()
{
	return mOnInternalResourceUpdated;
}
