#include <pch.h>
#include <aiva/utils/resource_barrier.h>

aiva::utils::ResourceBarrier::ResourceBarrier(D3D12_RESOURCE_STATES const state /*= D3D12_RESOURCE_STATE_COMMON*/)
{
	mMainState = state;
}

bool aiva::utils::ResourceBarrier::Transite(D3D12_RESOURCE_STATES const desiredState, std::optional<std::size_t> const subresource, D3D12_RESOURCE_STATES& previousState)
{
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

	if (!subresource)
	{
		if (mMainState == desiredState)
		{
			previousState = {};
			return false;
		}

		previousState = mMainState;
		mMainState = desiredState;
		return true;
	}
	else
	{
		auto const& subStateIter = mSubStates.find(*subresource);
		if (subStateIter == mSubStates.end())
		{
			previousState = mMainState;
			mSubStates.insert_or_assign(*subresource, desiredState);
			return true;
		}

		if (subStateIter->second == desiredState)
		{
			previousState = {};
			return false;
		}

		previousState = subStateIter->second;
		mSubStates.insert_or_assign(*subresource, desiredState);
		return true;
	}
}
