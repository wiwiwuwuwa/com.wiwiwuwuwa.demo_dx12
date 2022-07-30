#include <pch.h>
#include <aiva/layer1/grv_dsv_to_texture_2d.h>

#include <aiva/layer1/e_gpu_descriptor_heap_type.h>
#include <aiva/layer1/e_gpu_resource_view_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_texture_2d.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::GrvDsvToTexture2D::GrvDsvToTexture2D(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
}

aiva::layer1::GrvDsvToTexture2D::~GrvDsvToTexture2D()
{
	TerminateCacheUpdater();
}

aiva::layer1::GrvDsvToTexture2D::CacheUpdaterType& aiva::layer1::GrvDsvToTexture2D::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::GrvDsvToTexture2D::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::GrvDsvToTexture2D::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

aiva::layer1::EGpuDescriptorHeapType aiva::layer1::GrvDsvToTexture2D::HeapType() const
{
	return EGpuDescriptorHeapType::Dsv;
}

aiva::layer1::EGpuResourceViewType aiva::layer1::GrvDsvToTexture2D::ViewType() const
{
	return EGpuResourceViewType::Dsv;
}

void aiva::layer1::GrvDsvToTexture2D::CreateView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) const
{
	CacheUpdater().FlushChanges();

	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	auto const& aivaDesc = Desc();
	aiva::utils::Asserts::CheckBool(aivaDesc);

	auto const& aivaBuffer = aivaDesc->Resource;
	aiva::utils::Asserts::CheckBool(aivaBuffer);

	auto const& directxBuffer = aivaBuffer->InternalResource();
	winrt::check_bool(directxBuffer);

	auto const& directxDesc = InternalResource();
	aiva::utils::Asserts::CheckBool(directxDesc);

	device->CreateDepthStencilView(directxBuffer.get(), &directxDesc.value(), destination);
}

aiva::utils::TEvAction<aiva::utils::ECacheFlags>& aiva::layer1::GrvDsvToTexture2D::OnMarkAsChanged()
{
	return CacheUpdater().OnMarkAsChanged();
}

std::optional<aiva::layer1::GrvDsvToTexture2DDesc> const& aiva::layer1::GrvDsvToTexture2D::Desc() const
{
	return mDesc;
}

aiva::layer1::GrvDsvToTexture2D& aiva::layer1::GrvDsvToTexture2D::Desc(std::optional<GrvDsvToTexture2DDesc> const& desc)
{
	if (mDesc)
	{
		aiva::utils::Asserts::CheckBool(mDesc->Resource);
		mDesc->Resource->CacheUpdater().OnMarkAsChanged().disconnect(boost::bind(&GrvDsvToTexture2D::OnDescResourceMarkedAsChanged, this));
	}

	mDesc = desc;
	CacheUpdater().MarkAsChanged();

	if (mDesc)
	{
		aiva::utils::Asserts::CheckBool(mDesc->Resource);
		mDesc->Resource->CacheUpdater().OnMarkAsChanged().connect(boost::bind(&GrvDsvToTexture2D::OnDescResourceMarkedAsChanged, this));
	}

	return *this;
}

void aiva::layer1::GrvDsvToTexture2D::OnDescResourceMarkedAsChanged() const
{
	CacheUpdater().MarkAsChanged();
}

std::optional<D3D12_DEPTH_STENCIL_VIEW_DESC> aiva::layer1::GrvDsvToTexture2D::InternalResource() const
{
	auto const& aivaViewDesc = Desc();
	if (!aivaViewDesc)
	{
		return {};
	}

	auto const& aivaResource = aivaViewDesc->Resource;
	aiva::utils::Asserts::CheckBool(aivaResource);

	auto const& aivaResourceDesc = aivaResource->Desc();
	aiva::utils::Asserts::CheckBool(aivaResourceDesc);
	aiva::utils::Asserts::CheckBool(aivaResourceDesc->SupportDepthStencil);

	auto const& directxResource = aivaResource->InternalResource();
	winrt::check_bool(directxResource);

	auto const& directxResourceDesc = directxResource->GetDesc();

	auto directxViewDesc = D3D12_DEPTH_STENCIL_VIEW_DESC{};
	directxViewDesc.Format = directxResourceDesc.Format;
	directxViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	directxViewDesc.Flags = D3D12_DSV_FLAG_NONE;
	directxViewDesc.Texture2D.MipSlice = aivaViewDesc->MipLevel;

	return directxViewDesc;
}
