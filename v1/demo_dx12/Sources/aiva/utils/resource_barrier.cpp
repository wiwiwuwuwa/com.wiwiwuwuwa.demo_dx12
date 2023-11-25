#include <pch.h>
#include <aiva/utils/resource_barrier.h>

#include <aiva/utils/asserts.h>

aiva::utils::ResourceBarrier::ResourceBarrier(D3D12_RESOURCE_STATES const state /*= D3D12_RESOURCE_STATE_COMMON*/)
{
	mMainState = state;
}

bool aiva::utils::ResourceBarrier::Transite(D3D12_RESOURCE_STATES const desiredState, std::optional<std::size_t> const subresource, D3D12_RESOURCE_STATES& previousState)
{
	Asserts::CheckBool(!subresource || subresource.value() == 0, "Resource barrier is not implemented for use in subresources");

	if (mMainState == D3D12_RESOURCE_STATE_GENERIC_READ)
	{
		previousState = {};
		return false;
	}

	if (mMainState == D3D12_RESOURCE_STATE_COPY_DEST)
	{
		previousState = {};
		return false;
	}

	if (mMainState == desiredState)
	{
		previousState = {};
		return false;
	}

	previousState = mMainState;
	mMainState = desiredState;
	return true;
}
