#include <pch.h>
#include <aiva/layer1/gr_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/resource_barrier.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	GrBuffer::GrBuffer(EngineType const& engine) : AGraphicResource(engine)
	{

	}

	GrBuffer::~GrBuffer()
	{

	}

	GrBuffer::MemoryTypeEnum GrBuffer::MemoryType() const
	{
		return mMemoryType;
	}

	GrBuffer& GrBuffer::MemoryType(MemoryTypeEnum const memoryType)
	{
		if (mMemoryType != memoryType)
		{
			mMemoryType = memoryType;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	std::size_t GrBuffer::Size() const
	{
		return mSize;
	}

	GrBuffer& GrBuffer::Size(std::size_t const size)
	{
		if (mSize != size)
		{
			mSize = size;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	bool GrBuffer::SupportShaderAtomics() const
	{
		return mSupportShaderAtomics;
	}

	GrBuffer& GrBuffer::SupportShaderAtomics(bool const support)
	{
		if (mSupportShaderAtomics != support)
		{
			mSupportShaderAtomics = support;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	bool GrBuffer::SupportUnorderedAccess() const
	{
		return mSupportUnorderedAccess;
	}

	GrBuffer& GrBuffer::SupportUnorderedAccess(bool const support)
	{
		if (mSupportUnorderedAccess != support)
		{
			mSupportUnorderedAccess = support;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	void GrBuffer::RefreshInternalResourceFromSelf(winrt::com_ptr<ID3D12Resource>& resource, ResourceBarrier& barrier)
	{
		Asserts::CheckBool(!(SupportShaderAtomics() && MemoryType() != MemoryTypeEnum::GpuOnly), "Shader atomics is not supported");
		Asserts::CheckBool(Size() > 0, "Size is not valid");

		auto const& device = Engine().GraphicHardware().Device();
		winrt::check_bool(device);

		auto heapProperties = D3D12_HEAP_PROPERTIES{};
		heapProperties.Type = ToInternalEnum(MemoryType());
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask = 0;
		heapProperties.VisibleNodeMask = 0;

		auto heapFlags = D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;
		heapFlags |= SupportShaderAtomics() ? D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS : heapFlags;

		auto resourceDesc = D3D12_RESOURCE_DESC{};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = 0;
		resourceDesc.Width = Size();
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc = { 1, 0 };
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		resourceDesc.Flags |= SupportUnorderedAccess() ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : resourceDesc.Flags;

		auto resourceStates = D3D12_RESOURCE_STATE_COMMON;
		resourceStates |= MemoryType() == MemoryTypeEnum::CpuToGpu ? D3D12_RESOURCE_STATE_GENERIC_READ : resourceStates;
		resourceStates |= MemoryType() == MemoryTypeEnum::GpuToCpu ? D3D12_RESOURCE_STATE_COPY_DEST : resourceStates;
		barrier = resourceStates;

		auto commitedResource = ResourceType{};
		winrt::check_hresult(device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&commitedResource)));
		resource = commitedResource;
	}

	void GrBuffer::RefreshSelfFromInternalResource(winrt::com_ptr<ID3D12Resource> const& resource)
	{
		winrt::check_bool(resource);

		auto heapProperties = D3D12_HEAP_PROPERTIES{};
		auto heapFlags = D3D12_HEAP_FLAGS{};
		winrt::check_hresult(resource->GetHeapProperties(&heapProperties, &heapFlags));

		MemoryType(FromInternalEnum(heapProperties.Type));
		SupportShaderAtomics(heapFlags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS);

		auto const resourceDesc = resource->GetDesc();
		Asserts::CheckBool(resourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER, "Resource is not a buffer");

		Size(resourceDesc.Width);
		SupportUnorderedAccess(resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	}
}
