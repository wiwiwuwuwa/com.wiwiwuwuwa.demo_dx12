#include <pch.h>
#include <aiva/layer1/gr_texture_2d_desc.h>

aiva::layer1::GrTexture2DDesc::GrTexture2DDesc()
{

}

aiva::layer1::GrTexture2DDesc::GrTexture2DDesc(winrt::com_ptr<ID3D12Resource> const& resource) : GrTexture2DDesc()
{
	winrt::check_bool(resource);

	auto heapProperties = D3D12_HEAP_PROPERTIES{};
	auto heapFlags = D3D12_HEAP_FLAGS{};
	winrt::check_hresult(resource->GetHeapProperties(&heapProperties, &heapFlags));

	SupportShaderAtomics = heapFlags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS;

	auto const& resourceDesc = resource->GetDesc();
	aiva::utils::Asserts::CheckBool(resourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D);

	BufferFormat = FromInternalEnum(resourceDesc.Format);
	Width = resourceDesc.Width;
	Height = resourceDesc.Height;
	SupportDepthStencil = resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	SupportMipMapping = resourceDesc.MipLevels != 1;
	SupportRenderTarget = resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	SupportUnorderedAccess = resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
}
