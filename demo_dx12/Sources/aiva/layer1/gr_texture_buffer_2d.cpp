#include <pch.h>
#include <aiva/layer1/gr_texture_buffer_2d.h>

aiva::layer1::GrTextureBuffer2D::GrTextureBuffer2D(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeLowLevelData();
}

aiva::layer1::GrTextureBuffer2D::~GrTextureBuffer2D()
{
	TerminateLowLevelData();
}

aiva::utils::TEvAction<aiva::layer1::GrTextureBuffer2D::EDirtyFlags>& aiva::layer1::GrTextureBuffer2D::OnFlushCompleted()
{
	return mCacheRefresh.OnFlushCompleted();
}

winrt::com_ptr<ID3D12Resource> const& aiva::layer1::GrTextureBuffer2D::InternalResource()
{
	mCacheRefresh.FlushChanges();

	winrt::com_ptr<ID3D12Resource> internalResource = mInternalResource;
	winrt::check_bool(internalResource);

	return internalResource;
}

void aiva::layer1::GrTextureBuffer2D::InitializeLowLevelData()
{
	mCacheRefresh.OnFlushRequested().connect(boost::bind(&aiva::layer1::GrTextureBuffer2D::RefreshLowLevelData, this, boost::placeholders::_1));
}

void aiva::layer1::GrTextureBuffer2D::TerminateLowLevelData()
{
	mCacheRefresh.OnFlushRequested().disconnect(boost::bind(&aiva::layer1::GrTextureBuffer2D::RefreshLowLevelData, this, boost::placeholders::_1));
}

void aiva::layer1::GrTextureBuffer2D::RefreshLowLevelData(EDirtyFlags const dirtyFlags)
{

}
