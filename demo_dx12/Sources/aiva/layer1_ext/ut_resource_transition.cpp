#include <pch.h>
#include <aiva/layer1_ext/ut_resource_transition.h>

namespace aiva::layer1_ext
{
	UtResourceTransition::UtResourceTransition()
	{

	}

	UtResourceTransition::UtResourceTransition(D3D12_RESOURCE_STATES const currentState, D3D12_RESOURCE_STATES const desiredState) : UtResourceTransition{ {}, currentState, desiredState }
	{

	}

	UtResourceTransition::UtResourceTransition(std::optional<std::size_t> const subResource, D3D12_RESOURCE_STATES const currentState, D3D12_RESOURCE_STATES const desiredState) : mSubResource{ subResource }, mCurrentState{ currentState }, mDesiredState{ desiredState }
	{

	}

	UtResourceTransition::~UtResourceTransition()
	{

	}
}
