#include <pch.h>
#include <aiva/layer1/grv_cbv_to_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
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
	auto const aivaViewDesc = Desc();
	if (!aivaViewDesc.Resource)
	{
		return {};
	}

	auto const aivaResource = aivaViewDesc.Resource;
	aiva::utils::Asserts::CheckBool(aivaResource);

	auto const directxResource = aivaResource->InternalResource();
	winrt::check_bool(directxResource);

	auto const directxResourceDesc = directxResource->GetDesc();
	aiva::utils::Asserts::CheckBool(directxResourceDesc.Width > 0);

	D3D12_CONSTANT_BUFFER_VIEW_DESC directxViewDesc{};
	directxViewDesc.BufferLocation = directxResource->GetGPUVirtualAddress();
	directxViewDesc.SizeInBytes = directxResourceDesc.Width;

	return directxViewDesc;
}

aiva::utils::EvAction& aiva::layer1::GrvCbvToBuffer::OnInternalResourceUpdated()
{
	return mOnInternalResourceUpdated;
}

void aiva::layer1::GrvCbvToBuffer::NotifyInternalResourceUpdated()
{
	OnInternalResourceUpdated()();
}
