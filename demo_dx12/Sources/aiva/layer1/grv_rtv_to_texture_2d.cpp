#include <pch.h>
#include <aiva/layer1/grv_rtv_to_texture_2d.h>

#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/e_resource_view_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_texture_2d.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/graphic_resource_factory.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::GrvRtvToTexture2D::GrvRtvToTexture2D(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
}

aiva::layer1::GrvRtvToTexture2D::GrvRtvToTexture2D(Engine const& engine, GrvRtvToTexture2DDesc const& desc) : GrvRtvToTexture2D(engine)
{
	Desc(desc);
}

aiva::layer1::GrvRtvToTexture2D::GrvRtvToTexture2D(Engine const& engine, std::shared_ptr<GrTexture2D> const& resource) : GrvRtvToTexture2D(engine)
{
	auto desc = GrvRtvToTexture2DDesc{};
	desc.Resource = resource;
	desc.MipLevel = 0;

	Desc(desc);
}

aiva::layer1::GrvRtvToTexture2D::GrvRtvToTexture2D(Engine const& engine, winrt::com_ptr<ID3D12Resource> const& resource) : GrvRtvToTexture2D(engine)
{
	auto texture = GraphicResourceFactory::Create<GrTexture2D>(mEngine);
	texture->InternalResource(resource);

	auto desc = GrvRtvToTexture2DDesc{};
	desc.Resource = texture;
	desc.MipLevel = 0;

	Desc(desc);
}

aiva::layer1::GrvRtvToTexture2D::~GrvRtvToTexture2D()
{
	TerminateCacheUpdater();
}

aiva::layer1::GrvRtvToTexture2D::CacheUpdaterType& aiva::layer1::GrvRtvToTexture2D::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::GrvRtvToTexture2D::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::GrvRtvToTexture2D::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

aiva::layer1::EDescriptorHeapType aiva::layer1::GrvRtvToTexture2D::HeapType() const
{
	return EDescriptorHeapType::Rtv;
}

aiva::layer1::EResourceViewType aiva::layer1::GrvRtvToTexture2D::ViewType() const
{
	return EResourceViewType::Rtv;
}

void aiva::layer1::GrvRtvToTexture2D::CreateView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) const
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

	device->CreateRenderTargetView(directxBuffer.get(), &directxDesc.value(), destination);
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::GrvRtvToTexture2D::PrepareBarriers(bool const active) const
{
	CacheUpdater().FlushChanges();

	auto const& desc = Desc();
	aiva::utils::Asserts::CheckBool(desc);

	auto const& res = desc.value().Resource;
	aiva::utils::Asserts::CheckBool(res);

	auto const& state = active ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_COMMON;
	return res->PrepareBarriers(state, desc->MipLevel);
}

aiva::utils::TEvAction<aiva::utils::ECacheFlags>& aiva::layer1::GrvRtvToTexture2D::OnMarkAsChanged()
{
	return CacheUpdater().OnMarkAsChanged();
}

std::optional<aiva::layer1::GrvRtvToTexture2DDesc> const& aiva::layer1::GrvRtvToTexture2D::Desc() const
{
	return mDesc;
}

aiva::layer1::GrvRtvToTexture2D& aiva::layer1::GrvRtvToTexture2D::Desc(std::optional<GrvRtvToTexture2DDesc> const& desc)
{
	if (mDesc)
	{
		aiva::utils::Asserts::CheckBool(mDesc->Resource);
		mDesc->Resource->OnMarkAsChanged().disconnect(boost::bind(&GrvRtvToTexture2D::OnDescResourceMarkedAsChanged, this));
	}

	mDesc = desc;
	CacheUpdater().MarkAsChanged();

	if (mDesc)
	{
		aiva::utils::Asserts::CheckBool(mDesc->Resource);
		mDesc->Resource->OnMarkAsChanged().connect(boost::bind(&GrvRtvToTexture2D::OnDescResourceMarkedAsChanged, this));
	}

	return *this;
}

void aiva::layer1::GrvRtvToTexture2D::OnDescResourceMarkedAsChanged() const
{
	CacheUpdater().MarkAsChanged();
}

std::optional<D3D12_RENDER_TARGET_VIEW_DESC> aiva::layer1::GrvRtvToTexture2D::InternalResource() const
{
	auto const& aivaViewDesc = Desc();
	if (!aivaViewDesc)
	{
		return {};
	}

	auto const& aivaResource = aivaViewDesc->Resource;
	aiva::utils::Asserts::CheckBool(aivaResource);
	aiva::utils::Asserts::CheckBool(aivaResource->SupportRenderTarget());

	auto const& directxResource = aivaResource->InternalResource();
	winrt::check_bool(directxResource);

	auto const& directxResourceDesc = directxResource->GetDesc();

	auto directxViewDesc = D3D12_RENDER_TARGET_VIEW_DESC{};
	directxViewDesc.Format = directxResourceDesc.Format;
	directxViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	directxViewDesc.Texture2D.MipSlice = aivaViewDesc->MipLevel;
	directxViewDesc.Texture2D.PlaneSlice = 0;

	return directxViewDesc;
}
