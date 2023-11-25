#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/rt_find_resource_type.h>
#include <aiva/layer1_ext/rt_find_view_layout_a.h>
#include <aiva/layer1_ext/rt_find_view_params_b.h>
#include <aiva/layer1_ext/rt_is_heap_have_view.h>
#include <aiva/layer1_ext/rt_resource_descriptor_creator_fwd.h>
#include <aiva/layer1_ext/rt_resource_heap_container.h>
#include <aiva/layer1_ext/rt_resource_heap_type.h>
#include <aiva/layer1_ext/rt_resource_view_type.h>
#include <aiva/layer1_ext/rt_table_heap_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_val_dirty_flags_eager.h>
#include <aiva/utils_ext/m_object_field_val_dirty_flags_lazy.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/m_object_field_var_custom.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	template <RtResourceHeapType TResourceHeapType>
	struct RtTableHeap final : public aiva::utils_ext::TObject<RtTableHeap<TResourceHeapType>>, public RtResourceHeapContainer
	{
	// ----------------------------------------------------
	// Main

	public:
		using LayoutType = RtFindViewLayoutAT<TResourceHeapType>;

		template <typename TResourceClass, RtResourceViewType TResourceViewType>
		using ParamsType = RtFindViewParamsBT<TResourceClass, TResourceViewType>;

	public:
		M_OBJECT_BODY(RtTableHeap<TResourceHeapType>);

	protected:
		RtTableHeap(aiva::layer1::Engine& engine);

	public:
		~RtTableHeap() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER();

	private:
		void InitGenericFields();

		void ShutGenericFields();

	private:
		void GenericFields_When_NativeResourceFields_OnDirtied();

	// ----------------------------------------------------
	// Native Resource Fields

	public:
		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_LAZY_2(DirtyFlagsForNativeResource, private);

		M_OBJECT_FIELD_VAR_CUSTOM_4(winrt::com_ptr<ID3D12DescriptorHeap>, NativeResource, public, private);

	private:
		void InitNativeResourceFields();

		void ShutNativeResourceFields();

	private:
		void NativeResourceFields_When_ResourceViewsFields_OnDirtied();

		void NativeResourceFields_When_NativeResourceFields_OnFlushed();

	// ----------------------------------------------------
	// Resource Views Fields

	public:
		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER_2(DirtyFlagsForResourceViews, private);

		M_OBJECT_FIELD_VAR_3(private, M_SINGLE_ARG(std::map<LayoutType, std::shared_ptr<RtResourceDescriptorCreator>>), ResourceViews);

	public:
		template <RtResourceViewType TResourceViewType, typename TResourceClass, std::enable_if_t<RtIsHeapHaveViewV<TResourceHeapType, TResourceViewType>, bool> = true>
		void InsertResource(std::shared_ptr<TResourceClass> const& resource, LayoutType const& layout, ParamsType<TResourceClass, TResourceViewType> const& params);

		void RemoveResource(LayoutType const& layout);

		void ClearResources();

		std::size_t Size() const;

		bool Empty() const;

	private:
		void InitResourceViewsFields();

		void ShutResourceViewsFields();

	private:
		void ResourceViewsFields_When_ResourceView_OnDirtied();

	// ----------------------------------------------------
	// RtResourceHeapContainer

	public:
		winrt::com_ptr<ID3D12DescriptorHeap> const& GetResourceHeapData() override;

		aiva::utils_ext::TDirtyFlagsEagerReadOnly<>& GetResourceHeapDirtyFlags() override;

	// ----------------------------------------------------
	// Resources Barriers Emitter

	public:
		std::vector<D3D12_RESOURCE_BARRIER> TransiteResourceBarriers(UtBarriersFlags const flags) override;
	};
}

// --------------------------------------------------------

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1_ext/dx_descriptor_heap_type_utils.h>
#include <aiva/layer1_ext/rt_parse_descriptor_heap_type.h>
#include <aiva/layer1_ext/rt_resource_descriptor_creator.h>
#include <aiva/layer1_ext/rt_resource_view.h>
#include <aiva/layer1_ext/rt_support_shader_access.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	template <RtResourceHeapType TResourceHeapType>
	RtTableHeap<TResourceHeapType>::RtTableHeap(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitGenericFields();
		InitNativeResourceFields();
		InitResourceViewsFields();
	}

	template <RtResourceHeapType TResourceHeapType>
	RtTableHeap<TResourceHeapType>::~RtTableHeap()
	{
		ShutResourceViewsFields();
		ShutNativeResourceFields();
		ShutGenericFields();
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableHeap<TResourceHeapType>::InitGenericFields()
	{
		mDirtyFlagsForNativeResource.OnAfterDirtied().AttachListener(&ThisType::GenericFields_When_NativeResourceFields_OnDirtied, this);
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableHeap<TResourceHeapType>::ShutGenericFields()
	{
		mDirtyFlagsForNativeResource.OnAfterDirtied().RemoveListener(&ThisType::GenericFields_When_NativeResourceFields_OnDirtied, this);
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableHeap<TResourceHeapType>::GenericFields_When_NativeResourceFields_OnDirtied()
	{
		mDirtyFlags.Dirty();
	}

	template <RtResourceHeapType TResourceHeapType>
	winrt::com_ptr<ID3D12DescriptorHeap> const& RtTableHeap<TResourceHeapType>::NativeResource()
	{
		mDirtyFlagsForNativeResource.Flush();
		return mNativeResource;
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableHeap<TResourceHeapType>::NativeResource(winrt::com_ptr<ID3D12DescriptorHeap> const& value)
	{
		mNativeResource = value;
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableHeap<TResourceHeapType>::InitNativeResourceFields()
	{
		mDirtyFlagsForResourceViews.OnAfterDirtied().AttachListener(&ThisType::NativeResourceFields_When_ResourceViewsFields_OnDirtied, this);
		mDirtyFlagsForNativeResource.OnBeforeFlushed().AttachListener(&ThisType::NativeResourceFields_When_NativeResourceFields_OnFlushed, this);
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableHeap<TResourceHeapType>::ShutNativeResourceFields()
	{
		mDirtyFlagsForNativeResource.OnBeforeFlushed().RemoveListener(&ThisType::NativeResourceFields_When_NativeResourceFields_OnFlushed, this);
		mDirtyFlagsForResourceViews.OnAfterDirtied().RemoveListener(&ThisType::NativeResourceFields_When_ResourceViewsFields_OnDirtied, this);
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableHeap<TResourceHeapType>::NativeResourceFields_When_ResourceViewsFields_OnDirtied()
	{
		mDirtyFlagsForNativeResource.Dirty();
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableHeap<TResourceHeapType>::NativeResourceFields_When_NativeResourceFields_OnFlushed()
	{
		if (std::empty(mResourceViews))
		{
			mNativeResource = {};
			return;
		}

		auto const& device = Engine().GraphicHardware().Device();
		aiva::utils::Asserts::CheckBool(device, "Device is not valid");

		auto heapDesc = D3D12_DESCRIPTOR_HEAP_DESC{};
		heapDesc.Type = RtParseDescriptorHeapTypeV<TResourceHeapType>;
		heapDesc.NumDescriptors = std::size(mResourceViews);
		heapDesc.Flags = RtSupportShaderAccessV<TResourceHeapType> ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = {};

		winrt::check_hresult(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mNativeResource)));
		aiva::utils::Asserts::CheckBool(mNativeResource, "Native resource is not valid");

		auto const incrementSize = static_cast<std::size_t>(device->GetDescriptorHandleIncrementSize(heapDesc.Type));
		for (auto i = std::size_t{}; i < std::size(mResourceViews); i++)
		{
			auto const iter = std::next(std::cbegin(mResourceViews), i);
			aiva::utils::Asserts::CheckBool(iter != std::cend(mResourceViews));

			auto const& view = iter->second;
			aiva::utils::Asserts::CheckBool(view, "View is not valid");

			auto handle = mNativeResource->GetCPUDescriptorHandleForHeapStart();
			handle.ptr += i * incrementSize;

			view->CreateResourceDescriptorHandle(handle);
		}
	}

	template <RtResourceHeapType TResourceHeapType>
	template <RtResourceViewType TResourceViewType, typename TResourceClass, std::enable_if_t<RtIsHeapHaveViewV<TResourceHeapType, TResourceViewType>, bool>>
	void RtTableHeap<TResourceHeapType>::InsertResource(std::shared_ptr<TResourceClass> const& resource, LayoutType const& layout, ParamsType<TResourceClass, TResourceViewType> const& params)
	{
		aiva::utils::Asserts::CheckBool(resource, "Resource is not valid");

		static constexpr const auto ResourceType = RtFindResourceTypeV<TResourceClass>;
		using ResourceViewClass = RtResourceView<ResourceType, TResourceViewType>;

		auto const view = aiva::utils_ext::NewObject<ResourceViewClass>(resource, params, mEngine);
		aiva::utils::Asserts::CheckBool(view, "View is not valid");

		mResourceViews.emplace(layout, view);
		view->GetResourceDescriptorDirtyFlags().OnAfterDirtied().AttachListener(&ThisType::ResourceViewsFields_When_ResourceView_OnDirtied, this);

		mDirtyFlagsForResourceViews.Dirty();
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableHeap<TResourceHeapType>::RemoveResource(LayoutType const& layout)
	{
		auto const iter = mResourceViews.find(layout);
		aiva::utils::Asserts::CheckBool(iter != std::cend(mResourceViews), "Iter is not valid");

		auto const& view = iter->second;
		aiva::utils::Asserts::CheckBool(view, "View is not valid");

		view->GetResourceDescriptorDirtyFlags().OnAfterDirtied().RemoveListener(&ThisType::ResourceViewsFields_When_ResourceView_OnDirtied, this);
		mResourceViews.erase(iter);

		mDirtyFlagsForResourceViews.Dirty();
	}

	template <RtResourceHeapType TResourceHeapType>
	std::size_t RtTableHeap<TResourceHeapType>::Size() const
	{
		return std::size(mResourceViews);
	}

	template <RtResourceHeapType TResourceHeapType>
	bool RtTableHeap<TResourceHeapType>::Empty() const
	{
		return !Size();
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableHeap<TResourceHeapType>::ClearResources()
	{
		while (!std::empty(mResourceViews))
		{
			auto const iter = std::crbegin(mResourceViews);
			aiva::utils::Asserts::CheckBool(iter != std::crend(mResourceViews), "Iter is not valid");

			RemoveResource(iter->first);
		}

		mDirtyFlagsForResourceViews.Dirty();
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableHeap<TResourceHeapType>::InitResourceViewsFields()
	{
		ClearResources();
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableHeap<TResourceHeapType>::ShutResourceViewsFields()
	{
		ClearResources();
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableHeap<TResourceHeapType>::ResourceViewsFields_When_ResourceView_OnDirtied()
	{
		mDirtyFlagsForResourceViews.Dirty();
	}

	template <RtResourceHeapType TResourceHeapType>
	winrt::com_ptr<ID3D12DescriptorHeap> const& RtTableHeap<TResourceHeapType>::GetResourceHeapData()
	{
		return NativeResource();
	}

	template <RtResourceHeapType TResourceHeapType>
	aiva::utils_ext::TDirtyFlagsEagerReadOnly<>& RtTableHeap<TResourceHeapType>::GetResourceHeapDirtyFlags()
	{
		return DirtyFlags();
	}

	template <RtResourceHeapType TResourceHeapType>
	std::vector<D3D12_RESOURCE_BARRIER> RtTableHeap<TResourceHeapType>::TransiteResourceBarriers(UtBarriersFlags const flags)
	{
		auto resourcesBarriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		for (auto const& pair : mResourceViews)
		{
			auto const& resourceView = pair.second;
			aiva::utils::Asserts::CheckBool(resourceView, "Resource view is not valid");

			auto const resourceBarriers = resourceView->TransiteResourceBarriers(flags);
			std::copy(std::cbegin(resourceBarriers), std::cend(resourceBarriers), std::back_inserter(resourcesBarriers));
		}

		return resourcesBarriers;
	}
}
