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

aiva::layer1::EGpuFillMode aiva::layer1::ShaderPipelineDescriptor::FillMode() const
{
	CacheUpdater().FlushChanges();
	return mFillMode;
}

aiva::layer1::ShaderPipelineDescriptor& aiva::layer1::ShaderPipelineDescriptor::FillMode(EGpuFillMode const fillMode)
{
	mFillMode = fillMode;

	CacheUpdater().MarkAsChanged();
	return *this;
}

aiva::layer1::EGpuCullMode aiva::layer1::ShaderPipelineDescriptor::CullMode() const
{
	CacheUpdater().FlushChanges();
	return mCullMode;
}

aiva::layer1::ShaderPipelineDescriptor& aiva::layer1::ShaderPipelineDescriptor::CullMode(EGpuCullMode const cullMode)
{
	mCullMode = cullMode;

	CacheUpdater().MarkAsChanged();
	return *this;
}

bool aiva::layer1::ShaderPipelineDescriptor::DepthTest() const
{
	CacheUpdater().FlushChanges();
	return mDepthTest;
}

aiva::layer1::ShaderPipelineDescriptor& aiva::layer1::ShaderPipelineDescriptor::DepthTest(bool const depthTest)
{
	mDepthTest = depthTest;

	CacheUpdater().MarkAsChanged();
	return *this;
}

bool aiva::layer1::ShaderPipelineDescriptor::DepthWrite() const
{
	CacheUpdater().FlushChanges();
	return mDepthWrite;
}

aiva::layer1::ShaderPipelineDescriptor& aiva::layer1::ShaderPipelineDescriptor::DepthWrite(bool const depthWrite)
{
	mDepthWrite = depthWrite;

	CacheUpdater().MarkAsChanged();
	return *this;
}

aiva::layer1::EGpuComparisonFunc aiva::layer1::ShaderPipelineDescriptor::DepthFunc() const
{
	CacheUpdater().FlushChanges();
	return mDepthFunc;
}

aiva::layer1::ShaderPipelineDescriptor& aiva::layer1::ShaderPipelineDescriptor::DepthFunc(EGpuComparisonFunc const depthFunc)
{
	mDepthFunc = depthFunc;

	CacheUpdater().MarkAsChanged();
	return *this;
}

std::vector<aiva::layer1::EGpuResourceBufferFormat> aiva::layer1::ShaderPipelineDescriptor::RenderTargets() const
{
	CacheUpdater().FlushChanges();
	return mRenderTargets;
}

aiva::layer1::ShaderPipelineDescriptor& aiva::layer1::ShaderPipelineDescriptor::RenderTargets(std::vector<EGpuResourceBufferFormat> const& renderTargets)
{
	mRenderTargets = renderTargets;

	CacheUpdater().MarkAsChanged();
	return *this;
}

aiva::layer1::EGpuResourceBufferFormat aiva::layer1::ShaderPipelineDescriptor::DepthTarget() const
{
	CacheUpdater().FlushChanges();
	return mDepthTarget;
}

aiva::layer1::ShaderPipelineDescriptor& aiva::layer1::ShaderPipelineDescriptor::DepthTarget(EGpuResourceBufferFormat const depthTarget)
{
	mDepthTarget = depthTarget;

	CacheUpdater().MarkAsChanged();
	return *this;
}
