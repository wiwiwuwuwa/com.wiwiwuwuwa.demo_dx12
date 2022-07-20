#include <pch.h>
#include <aiva/layer1/grv_cbv_to_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GrvCbvToBuffer::GrvCbvToBuffer(Engine const& engine, GrvCbvToBufferDesc const& desc) : mEngine{ engine }
{
	Desc(desc);
}

aiva::layer1::GrvCbvToBuffer::~GrvCbvToBuffer()
{
	Desc({});
}

aiva::layer1::GrvCbvToBufferDesc const aiva::layer1::GrvCbvToBuffer::Desc() const
{
	return mDesc;
}

aiva::layer1::GrvCbvToBuffer& aiva::layer1::GrvCbvToBuffer::Desc(GrvCbvToBufferDesc const& desc)
{
	if (mDesc.Resource)
	{
		mDesc.Resource->OnInternalResourceUpdated().disconnect(boost::bind(&GrvCbvToBuffer::OnDescInternalResourceUpdated, this));
	}

	mDesc = desc;

	if (mDesc.Resource)
	{
		mDesc.Resource->OnInternalResourceUpdated().connect(boost::bind(&GrvCbvToBuffer::OnDescInternalResourceUpdated, this));
	}

	OnDescInternalResourceUpdated();
	return *this;
}

void aiva::layer1::GrvCbvToBuffer::OnDescInternalResourceUpdated()
{
	NotifyInternalResourceUpdated();
}

std::optional<D3D12_CONSTANT_BUFFER_VIEW_DESC> aiva::layer1::GrvCbvToBuffer::InternalResource()
{
	GrvCbvToBufferDesc const bufferDesc = Desc();
	if (!bufferDesc.Resource)
	{
		return {};
	}

	auto const device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	auto const resource = bufferDesc.Resource->InternalResource();
	winrt::check_bool(resource);

	D3D12_RESOURCE_DESC const resourceDesc = resource->GetDesc();
	D3D12_RESOURCE_ALLOCATION_INFO const allocationInfo = device->GetResourceAllocationInfo(0, 1, &resourceDesc);
	aiva::utils::Asserts::CheckBool(allocationInfo.SizeInBytes != UINT64_MAX);

	D3D12_CONSTANT_BUFFER_VIEW_DESC viewDesc{};
	viewDesc.BufferLocation = resource->GetGPUVirtualAddress();
	viewDesc.SizeInBytes = allocationInfo.SizeInBytes;

	return viewDesc;
}

aiva::utils::EvAction& aiva::layer1::GrvCbvToBuffer::OnInternalResourceUpdated()
{
	return mOnInternalResourceUpdated;
}

void aiva::layer1::GrvCbvToBuffer::NotifyInternalResourceUpdated()
{
	OnInternalResourceUpdated()();
}
