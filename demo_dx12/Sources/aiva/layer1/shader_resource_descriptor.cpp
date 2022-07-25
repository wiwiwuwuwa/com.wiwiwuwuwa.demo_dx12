#include <pch.h>
#include <aiva/layer1/shader_resource_descriptor.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/i_gpu_resource_view.h>
#include <aiva/utils/asserts.h>

aiva::layer1::ShaderResourceDescriptor::ShaderResourceDescriptor(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
}

aiva::layer1::ShaderResourceDescriptor::~ShaderResourceDescriptor()
{
	TerminateCacheUpdater();
}

aiva::utils::TCacheRefresh<aiva::layer1::ShaderResourceDescriptor::EDirtyFlags>& aiva::layer1::ShaderResourceDescriptor::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::ShaderResourceDescriptor::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<decltype(mCacheUpdater)::element_type>();
}

void aiva::layer1::ShaderResourceDescriptor::TerminateCacheUpdater()
{
	mCacheUpdater = {};
}

std::shared_ptr<aiva::layer1::IGpuResourceView> aiva::layer1::ShaderResourceDescriptor::ResourceView(std::string const& key) const
{
	auto const& resourceViewIter = mResourceViews.find(key);
	if (resourceViewIter == mResourceViews.end()) return {};

	return resourceViewIter->second;
}

aiva::layer1::ShaderResourceDescriptor& aiva::layer1::ShaderResourceDescriptor::ResourceView(std::string const& key, std::shared_ptr<IGpuResourceView> const& value)
{
	CacheUpdater().MarkAsChanged(EDirtyFlags::All);

	auto const& previousResourceIter = mResourceViews.find(key);
	if (previousResourceIter != mResourceViews.end())
	{
		auto const& previousResourceView = previousResourceIter->second;
		aiva::utils::Asserts::CheckBool(previousResourceView);

		previousResourceView->OnInternalResourceUpdated().disconnect(boost::bind(&ShaderResourceDescriptor::OnResourceViewUpdated, this));
		mResourceViews.erase(previousResourceIter);
	}

	if (!value)
	{
		return *this;
	}

	auto const& currentResourceIter = mResourceViews.insert_or_assign(key, value).first;
	if (currentResourceIter != mResourceViews.end())
	{
		auto const& currentResourceView = currentResourceIter->second;
		aiva::utils::Asserts::CheckBool(currentResourceView);

		currentResourceView->OnInternalResourceUpdated().connect(boost::bind(&ShaderResourceDescriptor::OnResourceViewUpdated, this));
	}
}

void aiva::layer1::ShaderResourceDescriptor::OnResourceViewUpdated()
{
	CacheUpdater().MarkAsChanged(EDirtyFlags::All);
}
