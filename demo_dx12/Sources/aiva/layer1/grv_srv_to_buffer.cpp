#include <pch.h>
#include <aiva/layer1/grv_srv_to_buffer.h>

#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/e_resource_view_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/shader_buffer.h>
#include <aiva/layer1/shader_struct.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::GrvSrvToBuffer::GrvSrvToBuffer(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
	InitializeBuffer();
	InitializeInternalResources();
}

aiva::layer1::GrvSrvToBuffer::GrvSrvToBuffer(Engine const& engine, GrvSrvToBufferDesc const& desc) : GrvSrvToBuffer(engine)
{
	Desc(desc);
}

aiva::layer1::GrvSrvToBuffer::~GrvSrvToBuffer()
{
	TerminateInternalResources();
	TerminateBuffer();
	TerminateCacheUpdater();
}

aiva::layer1::GrvSrvToBuffer::CacheUpdaterType& aiva::layer1::GrvSrvToBuffer::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::GrvSrvToBuffer::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::GrvSrvToBuffer::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

aiva::layer1::EDescriptorHeapType aiva::layer1::GrvSrvToBuffer::HeapType() const
{
	return EDescriptorHeapType::CbvSrvUav;
}

aiva::layer1::EResourceViewType aiva::layer1::GrvSrvToBuffer::ViewType() const
{
	return EResourceViewType::Srv;
}

void aiva::layer1::GrvSrvToBuffer::CreateView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) const
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

	device->CreateShaderResourceView(directxBuffer.get(), &directxDesc.value(), destination);
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::GrvSrvToBuffer::PrepareBarriers(bool const active) const
{
	CacheUpdater().FlushChanges();

	auto const& desc = Desc();
	aiva::utils::Asserts::CheckBool(desc);

	auto const& res = desc.value().Resource;
	aiva::utils::Asserts::CheckBool(res);

	auto const& state = active ? (D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) : D3D12_RESOURCE_STATE_COMMON;
	return res->PrepareBarriers(state);
}

aiva::utils::TEvAction<aiva::utils::ECacheFlags>& aiva::layer1::GrvSrvToBuffer::OnMarkAsChanged()
{
	return CacheUpdater().OnMarkAsChanged();
}

std::optional<aiva::layer1::GrvSrvToBufferDesc> const& aiva::layer1::GrvSrvToBuffer::Desc() const
{
	return mDesc;
}

aiva::layer1::GrvSrvToBuffer& aiva::layer1::GrvSrvToBuffer::Desc(std::optional<GrvSrvToBufferDesc> const& desc)
{
	if (mDesc)
	{
		Buffer().Struct({});

		aiva::utils::Asserts::CheckBool(mDesc->Resource);
		mDesc->Resource->CacheUpdater().OnMarkAsChanged().disconnect(boost::bind(&GrvSrvToBuffer::OnDescResourceMarkedAsChanged, this));
	}

	mDesc = desc;
	CacheUpdater().MarkAsChanged();

	if (mDesc)
	{
		aiva::utils::Asserts::CheckBool(mDesc->Resource);
		mDesc->Resource->CacheUpdater().OnMarkAsChanged().connect(boost::bind(&GrvSrvToBuffer::OnDescResourceMarkedAsChanged, this));

		aiva::utils::Asserts::CheckBool(mDesc->Struct);
		Buffer().Struct(mDesc->Struct);
	}

	return *this;
}

void aiva::layer1::GrvSrvToBuffer::OnDescResourceMarkedAsChanged() const
{
	CacheUpdater().MarkAsChanged();
}

aiva::layer1::ShaderBuffer& aiva::layer1::GrvSrvToBuffer::Buffer() const
{
	aiva::utils::Asserts::CheckBool(mBuffer);
	return *mBuffer;
}

void aiva::layer1::GrvSrvToBuffer::InitializeBuffer()
{
	mBuffer = ShaderBuffer::Create();
	aiva::utils::Asserts::CheckBool(mBuffer);

	mBuffer->CacheUpdater().OnMarkAsChanged().connect(boost::bind(&GrvSrvToBuffer::OnBufferMarkedAsChanged, this));
}

void aiva::layer1::GrvSrvToBuffer::TerminateBuffer()
{
	aiva::utils::Asserts::CheckBool(mBuffer);

	mBuffer->CacheUpdater().OnMarkAsChanged().disconnect(boost::bind(&GrvSrvToBuffer::OnBufferMarkedAsChanged, this));
	mBuffer = {};
}

void aiva::layer1::GrvSrvToBuffer::OnBufferMarkedAsChanged() const
{
	CacheUpdater().MarkAsChanged();
}

std::optional<D3D12_SHADER_RESOURCE_VIEW_DESC> aiva::layer1::GrvSrvToBuffer::InternalResource() const
{
	CacheUpdater().FlushChanges();

	auto const& aivaViewDesc = Desc();
	if (!aivaViewDesc)
	{
		return {};
	}

	auto const& aivaBuffer = mBuffer;
	aiva::utils::Asserts::CheckBool(aivaBuffer);

	auto const& aivaResource = aivaViewDesc->Resource;
	aiva::utils::Asserts::CheckBool(aivaResource);

	auto const& directxResource = aivaResource->InternalResource();
	winrt::check_bool(directxResource);

	auto const& directxResourceDesc = directxResource->GetDesc();

	auto directxViewDesc = D3D12_SHADER_RESOURCE_VIEW_DESC{};
	directxViewDesc.Format = directxResourceDesc.Format;
	directxViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	directxViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	directxViewDesc.Buffer.FirstElement = 0;
	directxViewDesc.Buffer.NumElements = aivaBuffer->Num();
	directxViewDesc.Buffer.StructureByteStride = aivaBuffer->ByteStride();
	directxViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	return directxViewDesc;
}

void aiva::layer1::GrvSrvToBuffer::InitializeInternalResources()
{
	CacheUpdater().FlushExecutors().connect(boost::bind(&GrvSrvToBuffer::RefreshInternalResources, this));
}

void aiva::layer1::GrvSrvToBuffer::TerminateInternalResources()
{
	CacheUpdater().FlushExecutors().disconnect(boost::bind(&GrvSrvToBuffer::RefreshInternalResources, this));
}

void aiva::layer1::GrvSrvToBuffer::RefreshInternalResources()
{
	auto const& desc = Desc();
	if (!desc)
	{
		return;
	}

	auto const& buffer = mBuffer;
	aiva::utils::Asserts::CheckBool(buffer);

	auto const& binaryData = buffer->SerializeToBinary();
	aiva::utils::Asserts::CheckBool(!binaryData.empty());

	auto const& aivaResource = desc->Resource;
	aiva::utils::Asserts::CheckBool(aivaResource);

	auto const& needCreateAivaResourceDesc = !aivaResource->Desc();
	if (needCreateAivaResourceDesc)
	{
		auto tempDesc = GrBufferDesc{};
		tempDesc.MemoryType = EResourceMemoryType::CpuToGpu;
		tempDesc.Size = binaryData.size();
		tempDesc.SupportShaderAtomics = false;
		tempDesc.SupportUnorderedAccess = false;

		aivaResource->Desc(tempDesc);
	}

	auto const& currentAivaResourceDesc = aivaResource->Desc();
	aiva::utils::Asserts::CheckBool(currentAivaResourceDesc);
	
	auto const& needUpdateAivaResourceDesc = currentAivaResourceDesc->MemoryType != EResourceMemoryType::CpuToGpu || currentAivaResourceDesc->Size != binaryData.size();
	if (needUpdateAivaResourceDesc)
	{
		auto tempDesc = currentAivaResourceDesc;
		tempDesc->Size = binaryData.size();

		aivaResource->Desc(tempDesc);
	}

	auto const& directxResource = aivaResource->InternalResource();
	winrt::check_bool(directxResource);

	void* destinationMemory{};
	winrt::check_hresult(directxResource->Map(0, nullptr, &destinationMemory));
	aiva::utils::Asserts::CheckBool(destinationMemory);
	aiva::utils::Asserts::CheckBool(memcpy_s(destinationMemory, binaryData.size(), binaryData.data(), binaryData.size()) == 0);
	directxResource->Unmap(0, nullptr);
}
