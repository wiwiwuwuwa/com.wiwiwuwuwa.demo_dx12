#include <pch.h>
#include <aiva/layer1/grv_cbv_to_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/shader_struct.h>
#include <aiva/layer1/shader_buffer.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GrvCbvToBuffer::GrvCbvToBuffer(Engine const& engine, GrvCbvToBufferDesc const& desc) : mEngine{ engine }
{
	Desc(desc);
}

aiva::layer1::GrvCbvToBuffer::~GrvCbvToBuffer()
{
	Desc({});
}

aiva::layer1::GrvCbvToBufferDesc const& aiva::layer1::GrvCbvToBuffer::Desc() const
{
	return mDesc;
}

aiva::layer1::GrvCbvToBuffer& aiva::layer1::GrvCbvToBuffer::Desc(GrvCbvToBufferDesc const& desc)
{
	auto const previousDesc = Desc();
	auto const desiredDesc = desc;

	mDesc = desiredDesc;
	RefreshInternalResourceUpdated(previousDesc, desiredDesc);

	return *this;
}

aiva::utils::EvAction& aiva::layer1::GrvCbvToBuffer::OnInternalResourceUpdated()
{
	return mOnInternalResourceUpdated;
}

void aiva::layer1::GrvCbvToBuffer::RefreshInternalResourceUpdated(GrvCbvToBufferDesc const& previousDesc, GrvCbvToBufferDesc const& desiredDesc)
{
	if (previousDesc.Resource)
	{
		previousDesc.Resource->OnInternalResourceUpdated().disconnect(boost::bind(&GrvCbvToBuffer::NotifyInternalResourceUpdated, this));
	}

	if (desiredDesc.Resource)
	{
		desiredDesc.Resource->OnInternalResourceUpdated().connect(boost::bind(&GrvCbvToBuffer::NotifyInternalResourceUpdated, this));
	}

	NotifyInternalResourceUpdated();
}

void aiva::layer1::GrvCbvToBuffer::NotifyInternalResourceUpdated()
{
	OnInternalResourceUpdated()();
}

std::optional<D3D12_CONSTANT_BUFFER_VIEW_DESC> aiva::layer1::GrvCbvToBuffer::InternalResource() const
{
	auto const aivaViewDesc = Desc();
	if (!aivaViewDesc.Resource)
	{
		return {};
	}

	auto const& aivaResource = aivaViewDesc.Resource;
	aiva::utils::Asserts::CheckBool(aivaResource);

	auto const& directxResource = aivaResource->InternalResource();
	winrt::check_bool(directxResource);

	auto const& directxResourceDesc = directxResource->GetDesc();

	D3D12_CONSTANT_BUFFER_VIEW_DESC directxViewDesc{};
	directxViewDesc.BufferLocation = directxResource->GetGPUVirtualAddress();
	directxViewDesc.SizeInBytes = directxResourceDesc.Width;

	return directxViewDesc;
}
