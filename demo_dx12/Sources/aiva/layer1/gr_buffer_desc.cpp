#include <pch.h>
#include <aiva/layer1/gr_buffer_desc.h>

#include <aiva/utils/asserts.h>

aiva::layer1::GrBufferDesc::GrBufferDesc()
{

}

aiva::layer1::GrBufferDesc::GrBufferDesc(winrt::com_ptr<ID3D12Resource> const& resource) : GrBufferDesc()
{
	winrt::check_bool(resource);

	auto heapProperties = D3D12_HEAP_PROPERTIES{};
	auto heapFlags = D3D12_HEAP_FLAGS{};
	winrt::check_hresult(resource->GetHeapProperties(&heapProperties, &heapFlags));

	MemoryType = FromInternalEnum(heapProperties.Type);
	SupportShaderAtomics = heapFlags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS;

	auto const& resourceDesc = resource->GetDesc();
	aiva::utils::Asserts::CheckBool(resourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER);

	Size = resourceDesc.Width;
	SupportUnorderedAccess = resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
}
