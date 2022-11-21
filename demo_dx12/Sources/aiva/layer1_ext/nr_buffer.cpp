#include <pch.h>
#include <aiva/layer1_ext/nr_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	NrBuffer::NrBuffer(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitGenericFields();
		InitParamsFields();
		InitObjectFields();
	}

	NrBuffer::~NrBuffer()
	{
		ShutObjectFields();
		ShutParamsFields();
		ShutGenericFields();
	}

	void NrBuffer::InitGenericFields()
	{
		DirtyFlagsForParamsSetter().OnAfterDirtied().AttachListener(&ThisType::GenericFields_When_ParamsFields_OnDirtied, this);
		DirtyFlagsForObjectGetter().OnAfterDirtied().AttachListener(&ThisType::GenericFields_When_ObjectFields_OnDirtied, this);
	}

	void NrBuffer::ShutGenericFields()
	{
		DirtyFlagsForObjectGetter().OnAfterDirtied().RemoveListener(&ThisType::GenericFields_When_ObjectFields_OnDirtied, this);
		DirtyFlagsForParamsSetter().OnAfterDirtied().RemoveListener(&ThisType::GenericFields_When_ParamsFields_OnDirtied, this);
	}

	void NrBuffer::GenericFields_When_ParamsFields_OnDirtied()
	{
		DirtyFlags().Dirty();
	}

	void NrBuffer::GenericFields_When_ObjectFields_OnDirtied()
	{
		DirtyFlags().Dirty();
	}

	aiva::layer1::EBufferMemoryType const& NrBuffer::MemoryType()
	{
		DirtyFlagsForParamsGetter().Flush();
		return mMemoryType;
	}

	void NrBuffer::MemoryType(aiva::layer1::EBufferMemoryType const& value)
	{
		if (mMemoryType != value)
		{
			mMemoryType = value;
			DirtyFlagsForParamsSetter().Dirty();
		}
	}

	std::size_t const& NrBuffer::Size()
	{
		DirtyFlagsForParamsGetter().Flush();
		return mSize;
	}

	void NrBuffer::Size(std::size_t const& value)
	{
		if (mSize != value)
		{
			mSize = value;
			DirtyFlagsForParamsSetter().Dirty();
		}
	}

	bool const& NrBuffer::SupportShaderAtomics()
	{
		DirtyFlagsForParamsGetter().Flush();
		return mSupportShaderAtomics;
	}

	void NrBuffer::SupportShaderAtomics(bool const& value)
	{
		if (mSupportShaderAtomics != value)
		{
			mSupportShaderAtomics = value;
			DirtyFlagsForParamsSetter().Dirty();
		}
	}

	bool const& NrBuffer::SupportUnorderedAccess()
	{
		DirtyFlagsForParamsGetter().Flush();
		return mSupportUnorderedAccess;
	}

	void NrBuffer::SupportUnorderedAccess(bool const& value)
	{
		if (mSupportUnorderedAccess != value)
		{
			mSupportUnorderedAccess = value;
			DirtyFlagsForParamsSetter().Dirty();
		}
	}

	void NrBuffer::InitParamsFields()
	{
		DirtyFlagsForObjectSetter().OnAfterDirtied().AttachListener(&ThisType::ParamsFields_When_ObjectFields_OnDirtied, this);
		DirtyFlagsForParamsGetter().OnBeforeFlushed().AttachListener(&ThisType::ParamsFields_When_ParamsFields_OnFlushed, this);
	}

	void NrBuffer::ShutParamsFields()
	{
		DirtyFlagsForParamsGetter().OnBeforeFlushed().RemoveListener(&ThisType::ParamsFields_When_ParamsFields_OnFlushed, this);
		DirtyFlagsForObjectSetter().OnAfterDirtied().RemoveListener(&ThisType::ParamsFields_When_ObjectFields_OnDirtied, this);
	}

	void NrBuffer::ParamsFields_When_ObjectFields_OnDirtied()
	{
		DirtyFlagsForParamsGetter().Dirty();
	}

	void NrBuffer::ParamsFields_When_ParamsFields_OnFlushed()
	{
		RefreshParamsFromObject();
	}

	void NrBuffer::RefreshParamsFromObject()
	{
		Asserts::CheckBool(mObject, "Object is not valid");

		auto heapProperties = D3D12_HEAP_PROPERTIES{};
		auto heapFlags = D3D12_HEAP_FLAGS{};
		winrt::check_hresult(mObject->GetHeapProperties(&heapProperties, &heapFlags));

		mMemoryType = aiva::layer1::FromInternalEnum(heapProperties.Type);
		mSupportShaderAtomics = heapFlags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS;

		auto const resourceDesc = mObject->GetDesc();
		Asserts::CheckBool(resourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER, "Resource is not a buffer");

		mSize = resourceDesc.Width;
		mSupportUnorderedAccess = resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}

	winrt::com_ptr<ID3D12Resource> const& NrBuffer::Object()
	{
		DirtyFlagsForObjectGetter().Flush();
		return mObject;
	}

	void NrBuffer::Object(winrt::com_ptr<ID3D12Resource> const& value)
	{
		if (mObject != value)
		{
			mObject = value;
			DirtyFlagsForObjectSetter().Dirty();
		}
	}

	void NrBuffer::InitObjectFields()
	{
		DirtyFlagsForParamsSetter().OnAfterDirtied().AttachListener(&ThisType::ObjectFields_When_ParamsFields_OnDirtied, this);
		DirtyFlagsForObjectGetter().OnBeforeFlushed().AttachListener(&ThisType::ObjectFields_When_ObjectFields_OnFlushed, this);
	}

	void NrBuffer::ShutObjectFields()
	{
		DirtyFlagsForObjectGetter().OnBeforeFlushed().RemoveListener(&ThisType::ObjectFields_When_ObjectFields_OnFlushed, this);
		DirtyFlagsForParamsSetter().OnAfterDirtied().RemoveListener(&ThisType::ObjectFields_When_ParamsFields_OnDirtied, this);
	}

	void NrBuffer::ObjectFields_When_ParamsFields_OnDirtied()
	{
		DirtyFlagsForObjectGetter().Dirty();
	}

	void NrBuffer::ObjectFields_When_ObjectFields_OnFlushed()
	{
		RefreshObjectFromParams();
	}

	void NrBuffer::RefreshObjectFromParams()
	{
		Asserts::CheckBool(!(mSupportShaderAtomics && mMemoryType != aiva::layer1::EBufferMemoryType::GpuOnly), "Shader atomics is not supported");
		Asserts::CheckBool(mSize > 0, "Size is not valid");

		auto const& device = Engine().GraphicHardware().Device();
		Asserts::CheckBool(device, "Device is not valid");

		auto heapProperties = D3D12_HEAP_PROPERTIES{};
		heapProperties.Type = aiva::layer1::ToInternalEnum(mMemoryType);
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask = 0;
		heapProperties.VisibleNodeMask = 0;

		auto heapFlags = D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;
		heapFlags |= mSupportShaderAtomics ? D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS : heapFlags;

		auto resourceDesc = D3D12_RESOURCE_DESC{};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = 0;
		resourceDesc.Width = mSize;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc = { 1, 0 };
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		resourceDesc.Flags |= mSupportUnorderedAccess ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : resourceDesc.Flags;

		auto resourceStates = D3D12_RESOURCE_STATE_COMMON;
		resourceStates |= mMemoryType == aiva::layer1::EBufferMemoryType::CpuToGpu ? D3D12_RESOURCE_STATE_GENERIC_READ : resourceStates;
		resourceStates |= mMemoryType == aiva::layer1::EBufferMemoryType::GpuToCpu ? D3D12_RESOURCE_STATE_COPY_DEST : resourceStates;
		mResourceBarriers = resourceStates;

		winrt::check_hresult(device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&mObject)));
		Asserts::CheckBool(mObject, "Object is not valid");
	}

	void NrBuffer::WriteBinaryData(boost::span<const std::byte> const& binaryData)
	{
		Asserts::CheckBool(!std::empty(binaryData), "Binary data is not valid");

		auto const& object = Object();
		Asserts::CheckBool(object, "Object is not valid");
		Asserts::CheckBool(MemoryType() == aiva::layer1::EBufferMemoryType::CpuToGpu, "Buffer doesn't support writing binary data");

		auto memory = std::add_pointer_t<void>{};
		winrt::check_hresult(object->Map({}, {}, &memory));

		Asserts::CheckBool(memory, "Memory ptr is not valid");
		Asserts::CheckBool(memcpy_s(memory, Size(), std::data(binaryData), std::size(binaryData)) == 0, "Failed to copy binary data");

		object->Unmap({}, {});
	}

	std::vector<D3D12_RESOURCE_BARRIER> NrBuffer::TransiteResourceBarriers(std::optional<std::size_t> const subResource, D3D12_RESOURCE_STATES const desiredState)
	{
		auto resourceBarriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		auto const& object = Object();
		if (object)
		{
			auto const resourceTransitions = mResourceBarriers.Transite(subResource, desiredState);
			for (auto const& resourceTransition : resourceTransitions)
			{
				auto& resourceBarrier = resourceBarriers.emplace_back();
				resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				resourceBarrier.Transition.pResource = mObject.get();
				resourceBarrier.Transition.Subresource = static_cast<UINT>(resourceTransition.SubResource().value_or(D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES));
				resourceBarrier.Transition.StateBefore = resourceTransition.CurrentState();
				resourceBarrier.Transition.StateAfter = resourceTransition.DesiredState();
			}

			auto const isUav = static_cast<bool>(desiredState & D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
			if (isUav)
			{
				auto& resourceBarrier = resourceBarriers.emplace_back();
				resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
				resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				resourceBarrier.UAV.pResource = mObject.get();
			}
		}

		return resourceBarriers;
	}
}
