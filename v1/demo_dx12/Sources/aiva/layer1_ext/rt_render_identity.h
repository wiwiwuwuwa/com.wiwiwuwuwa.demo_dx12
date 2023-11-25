#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/rt_find_resource_heap_type.h>
#include <aiva/layer1_ext/rt_find_view_params_b.h>
#include <aiva/layer1_ext/rt_render_identity_fwd.h>
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
	struct RtRenderIdentity final : public aiva::utils_ext::TObject<RtRenderIdentity<TResourceViewType>>, public UtResourcesBarriersEmitter
	{
	// ----------------------------------------------------
	// Main

	public:
		static constexpr const auto ResourceViewType = TResourceViewType;

		static constexpr const auto ResourceHeapType = RtFindResourceHeapTypeV<ResourceViewType>;

		template <typename TResourceClass>
		using ParamsType = RtFindViewParamsBT<TResourceClass, ResourceViewType>;

	public:
		M_OBJECT_BODY(RtRenderIdentity<TResourceViewType>);

	protected:
		RtRenderIdentity(aiva::layer1::Engine& engine);

	public:
		~RtRenderIdentity() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_3(private, std::shared_ptr<RtTableHeap<ResourceHeapType>>, ResourceTable);

	public:
		template <typename TResourceClass>
		void AttachResource(std::shared_ptr<TResourceClass> const& resource, ParamsType<TResourceClass> const& params) const;

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
	RtRenderIdentity<TResourceViewType>::RtRenderIdentity(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		mResourceTable = aiva::utils_ext::NewObject<decltype(mResourceTable)::element_type>(engine);
		aiva::utils::Asserts::CheckBool(mResourceTable, "Resource table is not valid");
	}

	template <RtResourceViewType TResourceViewType>
	RtRenderIdentity<TResourceViewType>::~RtRenderIdentity()
	{
		aiva::utils::Asserts::CheckBool(mResourceTable, "Resource table is not valid");
		mResourceTable = {};
	}

	template <RtResourceViewType TResourceViewType>
	template <typename TResourceClass>
	void RtRenderIdentity<TResourceViewType>::AttachResource(std::shared_ptr<TResourceClass> const& resource, ParamsType<TResourceClass> const& params) const
	{
		aiva::utils::Asserts::CheckBool(resource, "Resource is not valid");
		aiva::utils::Asserts::CheckBool(Empty(), "Resource is already attached");

		auto const& resourceTable = mResourceTable;
		aiva::utils::Asserts::CheckBool(resourceTable, "Resource table is not valid");

		resourceTable->InsertResource<ResourceViewType>(resource, {}, params);
	}

	template <RtResourceViewType TResourceViewType>
	bool RtRenderIdentity<TResourceViewType>::Empty() const
	{
		auto const& resourceTable = mResourceTable;
		aiva::utils::Asserts::CheckBool(resourceTable, "Resource table is not valid");

		return resourceTable->Empty();
	}

	template <RtResourceViewType TResourceViewType>
	D3D12_CPU_DESCRIPTOR_HANDLE RtRenderIdentity<TResourceViewType>::NativeHandleCPU() const
	{
		auto const& resourceTable = mResourceTable;
		aiva::utils::Asserts::CheckBool(resourceTable, "Resource table is not valid");

		auto const& nativeResource = resourceTable->NativeResource();
		aiva::utils::Asserts::CheckBool(nativeResource, "Native resource is not valid");

		return nativeResource->GetCPUDescriptorHandleForHeapStart();
	}

	template <RtResourceViewType TResourceViewType>
	D3D12_GPU_DESCRIPTOR_HANDLE RtRenderIdentity<TResourceViewType>::NativeHandleGPU() const
	{
		auto const& resourceTable = mResourceTable;
		aiva::utils::Asserts::CheckBool(resourceTable, "Resource table is not valid");

		auto const& nativeResource = resourceTable->NativeResource();
		aiva::utils::Asserts::CheckBool(nativeResource, "Native resource is not valid");

		return nativeResource->GetGPUDescriptorHandleForHeapStart();
	}

	template <RtResourceViewType TResourceViewType>
	std::vector<D3D12_RESOURCE_BARRIER> RtRenderIdentity<TResourceViewType>::TransiteResourceBarriers(UtBarriersFlags const flags)
	{
		auto const& resourceTable = mResourceTable;
		aiva::utils::Asserts::CheckBool(resourceTable, "Resource table is not valid");

		return resourceTable->TransiteResourceBarriers(flags);
	}
}
