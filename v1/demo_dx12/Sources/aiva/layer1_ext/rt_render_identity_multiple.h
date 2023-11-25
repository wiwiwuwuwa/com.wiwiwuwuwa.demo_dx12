#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/rt_find_resource_heap_type.h>
#include <aiva/layer1_ext/rt_find_view_layout_a.h>
#include <aiva/layer1_ext/rt_find_view_params_b.h>
#include <aiva/layer1_ext/rt_render_identity_multiple_fwd.h>
#include <aiva/layer1_ext/rt_resource_heap_type.h>
#include <aiva/layer1_ext/rt_table_heap_fwd.h>
#include <aiva/layer1_ext/rt_support_shader_access.h>
#include <aiva/layer1_ext/ut_resources_barriers_emitter.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	template <RtResourceViewType TResourceViewType>
	struct RtRenderIdentityMultiple final : public aiva::utils_ext::TObject<RtRenderIdentityMultiple<TResourceViewType>>, public UtResourcesBarriersEmitter
	{
	// ----------------------------------------------------
	// Main

	public:
		static constexpr const auto ResourceViewType = TResourceViewType;

		static constexpr const auto ResourceHeapType = RtFindResourceHeapTypeV<ResourceViewType>;

		using LayoutType = RtFindViewLayoutAT<ResourceHeapType>;

		template <typename TResourceClass>
		using ParamsType = RtFindViewParamsBT<TResourceClass, ResourceViewType>;

	public:
		M_OBJECT_BODY(RtRenderIdentityMultiple<TResourceViewType>);

	protected:
		RtRenderIdentityMultiple(aiva::layer1::Engine& engine);

	public:
		~RtRenderIdentityMultiple() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_3(private, std::shared_ptr<RtTableHeap<ResourceHeapType>>, ResourceTable);

	public:
		template <typename TResourceClass>
		void InsertResource(std::shared_ptr<TResourceClass> const& resource, LayoutType const& layout, ParamsType<TResourceClass> const& params) const;

		std::size_t Size() const;

		bool Empty() const;

	// ----------------------------------------------------
	// Native Fields

	public:
		D3D12_CPU_DESCRIPTOR_HANDLE NativeHandleCPU() const;

		D3D12_GPU_DESCRIPTOR_HANDLE NativeHandleGPU() const;

	// ----------------------------------------------------
	// Resources Barriers Emitter

	public:
		std::vector<D3D12_RESOURCE_BARRIER> TransiteResourceBarriers(UtBarriersFlags const flags) override;
	};
}

// --------------------------------------------------------

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/rt_table_heap.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	template <RtResourceViewType TResourceViewType>
	RtRenderIdentityMultiple<TResourceViewType>::RtRenderIdentityMultiple(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		mResourceTable = aiva::utils_ext::NewObject<decltype(mResourceTable)::element_type>(engine);
		aiva::utils::Asserts::CheckBool(mResourceTable, "Resource table is not valid");
	}

	template <RtResourceViewType TResourceViewType>
	RtRenderIdentityMultiple<TResourceViewType>::~RtRenderIdentityMultiple()
	{
		aiva::utils::Asserts::CheckBool(mResourceTable, "Resource table is not valid");
		mResourceTable = {};
	}

	template <RtResourceViewType TResourceViewType>
	template <typename TResourceClass>
	void RtRenderIdentityMultiple<TResourceViewType>::InsertResource(std::shared_ptr<TResourceClass> const& resource, LayoutType const& layout, ParamsType<TResourceClass> const& params) const
	{
		aiva::utils::Asserts::CheckBool(resource, "Resource is not valid");

		auto const& resourceTable = mResourceTable;
		aiva::utils::Asserts::CheckBool(resourceTable, "Resource table is not valid");

		resourceTable->InsertResource<ResourceViewType>(resource, layout, params);
	}

	template <RtResourceViewType TResourceViewType>
	std::size_t RtRenderIdentityMultiple<TResourceViewType>::Size() const
	{
		auto const& resourceTable = mResourceTable;
		aiva::utils::Asserts::CheckBool(resourceTable, "Resource table is not valid");

		return resourceTable->Size();
	}

	template <RtResourceViewType TResourceViewType>
	bool RtRenderIdentityMultiple<TResourceViewType>::Empty() const
	{
		return !Size();
	}

	template <RtResourceViewType TResourceViewType>
	D3D12_CPU_DESCRIPTOR_HANDLE RtRenderIdentityMultiple<TResourceViewType>::NativeHandleCPU() const
	{
		auto const& resourceTable = mResourceTable;
		aiva::utils::Asserts::CheckBool(resourceTable, "Resource table is not valid");

		auto const& nativeResource = resourceTable->NativeResource();
		aiva::utils::Asserts::CheckBool(nativeResource, "Native resource is not valid");

		return nativeResource->GetCPUDescriptorHandleForHeapStart();
	}

	template <RtResourceViewType TResourceViewType>
	D3D12_GPU_DESCRIPTOR_HANDLE RtRenderIdentityMultiple<TResourceViewType>::NativeHandleGPU() const
	{
		auto const& resourceTable = mResourceTable;
		aiva::utils::Asserts::CheckBool(resourceTable, "Resource table is not valid");

		auto const& nativeResource = resourceTable->NativeResource();
		aiva::utils::Asserts::CheckBool(nativeResource, "Native resource is not valid");

		return nativeResource->GetGPUDescriptorHandleForHeapStart();
	}

	template <RtResourceViewType TResourceViewType>
	std::vector<D3D12_RESOURCE_BARRIER> RtRenderIdentityMultiple<TResourceViewType>::TransiteResourceBarriers(UtBarriersFlags const flags)
	{
		auto const& resourceTable = mResourceTable;
		aiva::utils::Asserts::CheckBool(resourceTable, "Resource table is not valid");

		return resourceTable->TransiteResourceBarriers(flags);
	}
}
