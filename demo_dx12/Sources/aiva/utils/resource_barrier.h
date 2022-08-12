#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct ResourceBarrier final
	{
	public:
		ResourceBarrier(D3D12_RESOURCE_STATES const state = D3D12_RESOURCE_STATE_COMMON);

	public:
		bool Transite(D3D12_RESOURCE_STATES const desiredState, std::optional<std::size_t> const subresource, D3D12_RESOURCE_STATES& previousState);

	private:
		D3D12_RESOURCE_STATES mMainState{};
	};
}
