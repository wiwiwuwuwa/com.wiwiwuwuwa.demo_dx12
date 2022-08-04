#pragma once
#include <pch.h>

#include <aiva/utils/t_ev_action.h>

namespace aiva::layer1
{
	enum class EDescriptorHeapType : std::uint8_t;
	enum class EResourceViewType : std::uint8_t;
}

namespace aiva::utils
{
	enum class ECacheFlags : std::uint8_t;
}

namespace aiva::layer1
{
	struct IGpuResourceView
	{
	// ----------------------------------------------------
	// Main

	public:
		virtual ~IGpuResourceView() = default;

	// ----------------------------------------------------
	// Metadata

	public:
		virtual EDescriptorHeapType HeapType() const = 0;

		virtual EResourceViewType ViewType() const = 0;

	// ----------------------------------------------------
	// Actions

	public:
		virtual void CreateView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) const = 0;

		virtual std::vector<D3D12_RESOURCE_BARRIER> PrepareBarriers(bool const active) const = 0;

		virtual aiva::utils::TEvAction<aiva::utils::ECacheFlags>& OnMarkAsChanged() = 0;
	};
}
