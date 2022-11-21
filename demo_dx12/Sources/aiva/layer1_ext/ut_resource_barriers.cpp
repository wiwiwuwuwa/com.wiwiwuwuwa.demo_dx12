#include <pch.h>
#include <aiva/layer1_ext/ut_resource_barriers.h>

#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	UtResourceBarriers::UtResourceBarriers(D3D12_RESOURCE_STATES const initialState) : mMainState{ initialState }
	{
		mIsConstant |= (initialState & D3D12_RESOURCE_STATE_GENERIC_READ);
		mIsConstant |= (initialState & D3D12_RESOURCE_STATE_COPY_DEST);
	}

	UtResourceBarriers::~UtResourceBarriers()
	{

	}

	std::vector<UtResourceTransition> UtResourceBarriers::Transite(std::optional<std::size_t> const subResource, D3D12_RESOURCE_STATES const desiredState)
	{
		auto transitions = std::vector<UtResourceTransition>{};

		if (!mIsConstant)
		{
			transitions = TransiteUncheked(subResource, desiredState);
		}

		return transitions;
	}

	std::vector<UtResourceTransition> UtResourceBarriers::TransiteUncheked(std::optional<std::size_t> const subResource, D3D12_RESOURCE_STATES const desiredState)
	{
		auto transitions = std::vector<UtResourceTransition>{};

		if (subResource)
		{
			auto const transition = TransiteSubResource(subResource.value(), desiredState);
			if (transition)
			{
				transitions.emplace_back(transition.value());
			}
		}
		else
		{
			auto subResources = std::vector<std::size_t>{};
			std::transform(std::cbegin(mSubStates), std::cend(mSubStates), std::back_inserter(subResources), [](auto const& pair) { return pair.first; });

			for (auto const& subrs : subResources)
			{
				auto const transition = TransiteSubResource(subrs, mMainState);
				if (transition)
				{
					transitions.emplace_back(transition.value());
				}
			}

			{
				auto const transition = TransiteMainResource(desiredState);
				if (transition)
				{
					transitions.emplace_back(transition.value());
				}
			}
		}

		return transitions;
	}

	std::optional<UtResourceTransition> UtResourceBarriers::TransiteMainResource(D3D12_RESOURCE_STATES const desiredState)
	{
		auto const currentState = mMainState;
		if (currentState == desiredState)
		{
			return {};
		}

		mMainState = desiredState;
		return UtResourceTransition{ currentState, desiredState };
	}

	std::optional<UtResourceTransition> UtResourceBarriers::TransiteSubResource(std::size_t const subResource, D3D12_RESOURCE_STATES const desiredState)
	{
		auto const currentState = GetSubResourceState(subResource);
		if (currentState == desiredState)
		{
			return {};
		}

		SetSubResourceState(subResource, desiredState);
		return UtResourceTransition{ subResource, currentState, desiredState };
	}

	D3D12_RESOURCE_STATES UtResourceBarriers::GetSubResourceState(std::size_t const subResource) const
	{
		auto const iterator = mSubStates.find(subResource);
		if (iterator != std::cend(mSubStates))
		{
			return iterator->second;
		}
		else
		{
			return mMainState;
		}
	}

	void UtResourceBarriers::SetSubResourceState(std::size_t const subResource, D3D12_RESOURCE_STATES const desiredState)
	{
		if (desiredState == mMainState)
		{
			mSubStates.erase(subResource);
		}
		else
		{
			mSubStates.insert_or_assign(subResource, desiredState);
		}
	}
}
