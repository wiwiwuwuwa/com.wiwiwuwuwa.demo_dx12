#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_object.h>
#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/e_resource_view_type.h>

namespace aiva::layer1
{
	struct AGraphicResource;
}

namespace aiva::layer1
{
	struct AGraphicResourceView : public AGraphicObject
	{
	// ----------------------------------------------------
	// Main

	protected:
		AGraphicResourceView(aiva::layer1::Engine const& engine);

	public:
		~AGraphicResourceView() override;

	// ----------------------------------------------------
	// Internal Resource

	public:
		std::shared_ptr<AGraphicResource> const& InternalResource();

		void InternalResource(std::shared_ptr<AGraphicResource> const& resource);

	private:
		void InitializeInternalResource();

		void TerminateInternalResource();

	private:
		void ExecuteFlushForInternalResource();

		void ExecuteMarkAsChangedForSelf();

	private:
		std::shared_ptr<AGraphicResource> mInternalResource{};

	// ----------------------------------------------------
	// Metadata

	public:
		virtual EDescriptorHeapType HeapType() const = 0;

		virtual EResourceViewType ViewType() const = 0;

	// ----------------------------------------------------
	// DirectX

	public:
		virtual void CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) = 0;

		virtual std::vector<D3D12_RESOURCE_BARRIER> CreateDirectxBarriers(bool const active) = 0;
	};
}
