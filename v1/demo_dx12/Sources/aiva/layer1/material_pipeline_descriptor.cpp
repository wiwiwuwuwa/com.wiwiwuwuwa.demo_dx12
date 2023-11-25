#include <pch.h>
#include <aiva/layer1/material_pipeline_descriptor.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::MaterialPipelineDescriptor::MaterialPipelineDescriptor(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
}

aiva::layer1::MaterialPipelineDescriptor::~MaterialPipelineDescriptor()
{
	TerminateCacheUpdater();
}

aiva::layer1::MaterialPipelineDescriptor::CacheUpdaterType& aiva::layer1::MaterialPipelineDescriptor::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::MaterialPipelineDescriptor::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::MaterialPipelineDescriptor::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

aiva::layer1::EFillMode aiva::layer1::MaterialPipelineDescriptor::FillMode() const
{
	CacheUpdater().FlushChanges();
	return mFillMode;
}

aiva::layer1::MaterialPipelineDescriptor& aiva::layer1::MaterialPipelineDescriptor::FillMode(EFillMode const fillMode)
{
	mFillMode = fillMode;

	CacheUpdater().MarkAsChanged();
	return *this;
}

aiva::layer1::ECullMode aiva::layer1::MaterialPipelineDescriptor::CullMode() const
{
	CacheUpdater().FlushChanges();
	return mCullMode;
}

aiva::layer1::MaterialPipelineDescriptor& aiva::layer1::MaterialPipelineDescriptor::CullMode(ECullMode const cullMode)
{
	mCullMode = cullMode;

	CacheUpdater().MarkAsChanged();
	return *this;
}

bool aiva::layer1::MaterialPipelineDescriptor::DepthTest() const
{
	CacheUpdater().FlushChanges();
	return mDepthTest;
}

aiva::layer1::MaterialPipelineDescriptor& aiva::layer1::MaterialPipelineDescriptor::DepthTest(bool const depthTest)
{
	mDepthTest = depthTest;

	CacheUpdater().MarkAsChanged();
	return *this;
}

bool aiva::layer1::MaterialPipelineDescriptor::DepthWrite() const
{
	CacheUpdater().FlushChanges();
	return mDepthWrite;
}

aiva::layer1::MaterialPipelineDescriptor& aiva::layer1::MaterialPipelineDescriptor::DepthWrite(bool const depthWrite)
{
	mDepthWrite = depthWrite;

	CacheUpdater().MarkAsChanged();
	return *this;
}

aiva::layer1::EComparisonFunc aiva::layer1::MaterialPipelineDescriptor::DepthFunc() const
{
	CacheUpdater().FlushChanges();
	return mDepthFunc;
}

aiva::layer1::MaterialPipelineDescriptor& aiva::layer1::MaterialPipelineDescriptor::DepthFunc(EComparisonFunc const depthFunc)
{
	mDepthFunc = depthFunc;

	CacheUpdater().MarkAsChanged();
	return *this;
}

std::vector<aiva::layer1::EResourceBufferFormat> aiva::layer1::MaterialPipelineDescriptor::RenderTargets() const
{
	CacheUpdater().FlushChanges();
	return mRenderTargets;
}

aiva::layer1::MaterialPipelineDescriptor& aiva::layer1::MaterialPipelineDescriptor::RenderTargets(std::vector<EResourceBufferFormat> const& renderTargets)
{
	mRenderTargets = renderTargets;

	CacheUpdater().MarkAsChanged();
	return *this;
}

aiva::layer1::EResourceBufferFormat aiva::layer1::MaterialPipelineDescriptor::DepthTarget() const
{
	CacheUpdater().FlushChanges();
	return mDepthTarget;
}

aiva::layer1::MaterialPipelineDescriptor& aiva::layer1::MaterialPipelineDescriptor::DepthTarget(EResourceBufferFormat const depthTarget)
{
	mDepthTarget = depthTarget;

	CacheUpdater().MarkAsChanged();
	return *this;
}

void aiva::layer1::MaterialPipelineDescriptor::CopyPropertiesFrom(MaterialPipelineDescriptor const& source)
{
	FillMode(source.FillMode());
	CullMode(source.CullMode());
	DepthTest(source.DepthTest());
	DepthWrite(source.DepthWrite());
	DepthFunc(source.DepthFunc());
	RenderTargets(source.RenderTargets());
	DepthTarget(source.DepthTarget());

	CacheUpdater().MarkAsChanged();
}
