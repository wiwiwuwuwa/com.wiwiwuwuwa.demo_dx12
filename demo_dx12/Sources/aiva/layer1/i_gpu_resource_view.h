#pragma once
#include <pch.h>

#include <aiva/utils/t_ev_action.h>

namespace aiva::layer1
{
	enum class EGpuDescriptorHeapType : std::uint8_t;
	enum class EGpuResourceViewType : std::uint8_t;
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
		virtual EGpuDescriptorHeapType HeapType() const = 0;

		virtual EGpuResourceViewType ViewType() const = 0;

	// ----------------------------------------------------
	// Actions

	public:
		virtual void CreateView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) const = 0;

		virtual aiva::utils::TEvAction<aiva::utils::ECacheFlags>& OnMarkAsChanged() = 0;
	};
}
