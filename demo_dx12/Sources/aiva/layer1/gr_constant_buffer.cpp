#include <pch.h>
#include <aiva/layer1/gr_constant_buffer.h>

#include <aiva/layer1/constant_packer.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>

aiva::layer1::GrConstantBuffer::GrConstantBuffer(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeLowLevelData();
}

aiva::layer1::GrConstantBuffer::~GrConstantBuffer()
{
	TerminateLowLevelData();
}

aiva::utils::TEvAction<aiva::layer1::GrConstantBuffer::EDirtyFlags>& aiva::layer1::GrConstantBuffer::OnFlushCompleted()
{
	return mCacheRefresh.OnFlushCompleted();
}

winrt::com_ptr<ID3D12Resource> const& aiva::layer1::GrConstantBuffer::InternalResource()
{
	mCacheRefresh.FlushChanges();

	winrt::com_ptr<ID3D12Resource> internalResource = mInternalResource;
	winrt::check_bool(internalResource);

	return internalResource;
}

void aiva::layer1::GrConstantBuffer::InitializeLowLevelData()
{
	mCacheRefresh.OnFlushRequested().connect(boost::bind(&aiva::layer1::GrConstantBuffer::RefreshLowLevelData, this, boost::placeholders::_1));
}

void aiva::layer1::GrConstantBuffer::TerminateLowLevelData()
{
	mCacheRefresh.OnFlushRequested().disconnect(boost::bind(&aiva::layer1::GrConstantBuffer::RefreshLowLevelData, this, boost::placeholders::_1));
}

void aiva::layer1::GrConstantBuffer::RefreshLowLevelData(EDirtyFlags const dirtyFlags)
{
	std::vector<std::byte> const binaryData = aiva::layer1::ConstantPacker::PackConstants(mValues.cbegin(), mValues.cend());
	aiva::utils::Asserts::CheckBool(!binaryData.empty());

	RefreshResourceObject(binaryData);
	RefreshResourceData(binaryData);
}

void aiva::layer1::GrConstantBuffer::RefreshResourceObject(boost::span<const std::byte> const& binaryData)
{
	aiva::utils::Asserts::CheckBool(!binaryData.empty());

	bool const needRefresh = !mInternalResource || mInternalResource->GetDesc().Width != binaryData.size();
	if (!needRefresh)
	{
		return;
	}

	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = binaryData.size();
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc = { 1, 0 };
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_GENERIC_READ;

	winrt::com_ptr<ID3D12Resource> resourceObject{};
	winrt::check_hresult(device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&resourceObject)));

	winrt::check_bool(resourceObject);
	mInternalResource = resourceObject;
}

void aiva::layer1::GrConstantBuffer::RefreshResourceData(boost::span<const std::byte> const& binaryData)
{
	aiva::utils::Asserts::CheckBool(!binaryData.empty());

	winrt::com_ptr<ID3D12Resource> const resourceObject = mInternalResource;
	aiva::utils::Asserts::CheckBool(resourceObject);
	aiva::utils::Asserts::CheckBool(resourceObject->GetDesc().Width == binaryData.size());

	void* destinationMemory{};
	winrt::check_hresult(resourceObject->Map(0, nullptr, &destinationMemory));

	winrt::check_bool(destinationMemory);
	aiva::utils::Asserts::CheckBool(memcpy_s(destinationMemory, resourceObject->GetDesc().Width, binaryData.data(), binaryData.size()) == 0);

	resourceObject->Unmap(0, nullptr);
}
