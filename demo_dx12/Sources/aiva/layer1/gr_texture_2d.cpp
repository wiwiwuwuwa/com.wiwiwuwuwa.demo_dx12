#include <pch.h>
#include <aiva/layer1/gr_texture_2d.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GrTexture2D::GrTexture2D(aiva::layer1::Engine const& engine, GrTexture2DDesc const& desc) : mEngine{ engine }
{
	Desc(desc);
}

aiva::layer1::GrTexture2D::~GrTexture2D()
{

}

aiva::layer1::GrTexture2DDesc aiva::layer1::GrTexture2D::Desc() const
{
	winrt::com_ptr<ID3D12Resource> const resource = mInternalResource;
	winrt::check_bool(resource);

	GrTexture2DDesc desc{};

	{
		D3D12_HEAP_PROPERTIES heapProperties{};
		D3D12_HEAP_FLAGS heapFlags{};
		winrt::check_hresult(resource->GetHeapProperties(&heapProperties, &heapFlags));

		desc.SupportShaderAtomics = heapFlags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS;
	}

	{
		D3D12_RESOURCE_DESC resourceDesc = resource->GetDesc();

		desc.BufferFormat = FromInternalEnum(resourceDesc.Format);
		desc.Width = resourceDesc.Width;
		desc.Height = resourceDesc.Height;
		desc.SupportDepthStencil = resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		desc.SupportMipMapping = resourceDesc.MipLevels != 1;
		desc.SupportRenderTarget = resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		desc.SupportUnorderedAccess = resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}

	return desc;
}

aiva::layer1::GrTexture2D& aiva::layer1::GrTexture2D::Desc(GrTexture2DDesc const& desc)
{
	UpdateInternalResource(desc);
	return *this;
}

winrt::com_ptr<ID3D12Resource> const& aiva::layer1::GrTexture2D::InternalResource()
{
	winrt::com_ptr<ID3D12Resource> const resource = mInternalResource;
	winrt::check_bool(resource);

	return resource;
}

aiva::utils::EvAction& aiva::layer1::GrTexture2D::OnInternalResourceUpdated()
{
	return mOnInternalResourceUpdated;
}

void aiva::layer1::GrTexture2D::UpdateInternalResource(GrTexture2DDesc const& desc)
{
	aiva::utils::Asserts::CheckBool(desc.Width > 0);
	aiva::utils::Asserts::CheckBool(desc.Height > 0);
	aiva::utils::Asserts::CheckBool(desc.SupportDepthStencil == SupportsDepthStencil(desc.BufferFormat));
	aiva::utils::Asserts::CheckBool(!(desc.SupportMipMapping && desc.Width != desc.Height));
	aiva::utils::Asserts::CheckBool(desc.SupportRenderTarget != SupportsDepthStencil(desc.BufferFormat));
	aiva::utils::Asserts::CheckBool(!(desc.SupportRenderTarget && desc.SupportDepthStencil));
	aiva::utils::Asserts::CheckBool(!(desc.SupportDepthStencil && desc.SupportUnorderedAccess));

	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;
	heapFlags |= desc.SupportShaderAtomics ? D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS : heapFlags;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = desc.Width;
	resourceDesc.Height = desc.Height;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = desc.SupportMipMapping ? 0 : 1;
	resourceDesc.Format = ToInternalEnum(desc.BufferFormat);
	resourceDesc.SampleDesc = { 1, 0 };
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Flags |= desc.SupportRenderTarget ? D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET : resourceDesc.Flags;
	resourceDesc.Flags |= desc.SupportDepthStencil ? D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL : resourceDesc.Flags;
	resourceDesc.Flags |= desc.SupportUnorderedAccess ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : resourceDesc.Flags;

	D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COMMON;

	winrt::com_ptr<ID3D12Resource> resource{};
	winrt::check_hresult(device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&resource)));

	winrt::check_bool(resource);
	mInternalResource = resource;

	OnInternalResourceUpdated()();
}
