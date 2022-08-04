#include <pch.h>
#include <aiva/layer1/grv_cbv_to_buffer.h>

//#include <aiva/layer1/engine.h>
//#include <aiva/layer1/gr_buffer.h>
//#include <aiva/layer1/graphic_hardware.h>
//#include <aiva/layer1/shader_struct.h>
//#include <aiva/layer1/shader_buffer.h>
//#include <aiva/utils/asserts.h>
//
//aiva::layer1::GrvCbvToBuffer::GrvCbvToBuffer(Engine const& engine, GrvCbvToBufferDesc const& desc) : mEngine{ engine }
//{
//	InitializeStruct();
//	Desc(desc);
//}
//
//aiva::layer1::GrvCbvToBuffer::~GrvCbvToBuffer()
//{
//	Desc({});
//	TerminateStruct();
//}
//
//aiva::layer1::EDescriptorHeapType aiva::layer1::GrvCbvToBuffer::DescriptorHeapType() const
//{
//	return EDescriptorHeapType::CbvSrvUav;
//}
//
//aiva::layer1::EResourceViewType aiva::layer1::GrvCbvToBuffer::ResourceViewType() const
//{
//	return EResourceViewType::Cbv;
//}
//
//aiva::layer1::GrvCbvToBufferDesc const& aiva::layer1::GrvCbvToBuffer::Desc() const
//{
//	return mDesc;
//}
//
//aiva::layer1::GrvCbvToBuffer& aiva::layer1::GrvCbvToBuffer::Desc(GrvCbvToBufferDesc const& desc)
//{
//	auto const previousDesc = Desc();
//	auto const desiredDesc = desc;
//
//	mDesc = desiredDesc;
//	RefreshInternalResourceUpdated(previousDesc, desiredDesc);
//
//	return *this;
//}
//
//aiva::layer1::ShaderStruct& aiva::layer1::GrvCbvToBuffer::Struct() const
//{
//	aiva::utils::Asserts::CheckBool(mStruct);
//	return *mStruct;
//}
//
//aiva::layer1::GrvCbvToBuffer& aiva::layer1::GrvCbvToBuffer::ApplyChanges()
//{
//	aiva::utils::Asserts::CheckBool(mStruct);
//
//	auto const& desc = Desc();
//	aiva::utils::Asserts::CheckBool(desc.Resource);
//
//	auto const& aivaResource = desc.Resource;
//	aiva::utils::Asserts::CheckBool(aivaResource);
//
//	auto const& currentDirectxResource = aivaResource->InternalResource();
//	winrt::check_bool(currentDirectxResource);
//
//	auto const& currentDirectxDesc = currentDirectxResource->GetDesc();
//	aiva::utils::Asserts::CheckBool(currentDirectxDesc.Width > 0);
//
//	auto const& binaryData = (*ShaderBuffer::Create(mStruct)).Add(mStruct).SerializeToBinary();
//	aiva::utils::Asserts::CheckBool(binaryData.size() > 0);
//
//	auto const& needUpdateAivaDesc = currentDirectxDesc.Width != binaryData.size();
//	if (needUpdateAivaDesc)
//	{
//		auto updatedAivaDesc = aivaResource->Desc();
//		updatedAivaDesc.Size = binaryData.size();
//
//		aivaResource->Desc(updatedAivaDesc);
//	}
//
//	auto const& updatedDirectxResource = aivaResource->InternalResource();
//	winrt::check_bool(updatedDirectxResource);
//
//	void* destinationMemory{};
//	winrt::check_hresult(updatedDirectxResource->Map(0, nullptr, &destinationMemory));
//	aiva::utils::Asserts::CheckBool(destinationMemory);
//	aiva::utils::Asserts::CheckBool(memcpy_s(destinationMemory, binaryData.size(), binaryData.data(), binaryData.size()) == 0);
//	updatedDirectxResource->Unmap(0, nullptr);
//
//	return *this;
//}
//
//void aiva::layer1::GrvCbvToBuffer::InitializeStruct()
//{
//	mStruct = ShaderStruct::Create();
//}
//
//void aiva::layer1::GrvCbvToBuffer::TerminateStruct()
//{
//	mStruct = {};
//}
//
//aiva::utils::EvAction& aiva::layer1::GrvCbvToBuffer::OnInternalResourceUpdated()
//{
//	return mOnInternalResourceUpdated;
//}
//
//void aiva::layer1::GrvCbvToBuffer::RefreshInternalResourceUpdated(GrvCbvToBufferDesc const& previousDesc, GrvCbvToBufferDesc const& desiredDesc)
//{
//	if (previousDesc.Resource)
//	{
//		previousDesc.Resource->OnInternalResourceUpdated().disconnect(boost::bind(&GrvCbvToBuffer::NotifyInternalResourceUpdated, this));
//	}
//
//	if (desiredDesc.Resource)
//	{
//		desiredDesc.Resource->OnInternalResourceUpdated().connect(boost::bind(&GrvCbvToBuffer::NotifyInternalResourceUpdated, this));
//	}
//
//	NotifyInternalResourceUpdated();
//}
//
//void aiva::layer1::GrvCbvToBuffer::NotifyInternalResourceUpdated()
//{
//	OnInternalResourceUpdated()();
//}
//
//std::optional<D3D12_CONSTANT_BUFFER_VIEW_DESC> aiva::layer1::GrvCbvToBuffer::InternalResource() const
//{
//	auto const aivaViewDesc = Desc();
//	if (!aivaViewDesc.Resource)
//	{
//		return {};
//	}
//
//	auto const& aivaResource = aivaViewDesc.Resource;
//	aiva::utils::Asserts::CheckBool(aivaResource);
//
//	auto const& directxResource = aivaResource->InternalResource();
//	winrt::check_bool(directxResource);
//
//	auto const& directxResourceDesc = directxResource->GetDesc();
//
//	D3D12_CONSTANT_BUFFER_VIEW_DESC directxViewDesc{};
//	directxViewDesc.BufferLocation = directxResource->GetGPUVirtualAddress();
//	directxViewDesc.SizeInBytes = directxResourceDesc.Width;
//
//	return directxViewDesc;
//}
