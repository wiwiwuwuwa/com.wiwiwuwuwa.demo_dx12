#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/rt_find_resource_class.h>
#include <aiva/layer1_ext/rt_find_view_params_a.h>
#include <aiva/layer1_ext/rt_resource_descriptor_creator.h>
#include <aiva/layer1_ext/rt_resource_type.h>
#include <aiva/layer1_ext/rt_resource_view_type.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_val_dirty_flags_eager.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/m_object_field_var_custom.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	struct RtResourceView final : public aiva::utils_ext::TObject<RtResourceView<TResourceType, TResourceViewType>>, public RtResourceDescriptorCreator
	{
	// ----------------------------------------------------
	// Main

	public:
		using ResourceClass = RtFindResourceClassT<TResourceType>;

		using ParamsClass = RtFindViewParamsAT<TResourceType, TResourceViewType>;

	public:
		M_OBJECT_BODY(M_SINGLE_ARG(RtResourceView<TResourceType, TResourceViewType>));

	protected:
		RtResourceView(std::shared_ptr<ResourceClass> const& resource, ParamsClass const& params, aiva::layer1::Engine& engine);

	public:
		~RtResourceView() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER();

		M_OBJECT_FIELD_VAR_CUSTOM_4(std::shared_ptr<ResourceClass>, Resource, public, private);

		M_OBJECT_FIELD_VAR_4(public, private, ParamsClass, Params);

	private:
		void InitGenericFields(std::shared_ptr<ResourceClass> const& resource, ParamsClass const& params);

		void ShutGenericFields();

	private:
		void GenericFields_When_Resource_OnDirtied();

	// ----------------------------------------------------
	// Resource Descriptor Creator

	public:
		void CreateResourceDescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE const& outHandle) override;

		aiva::utils_ext::TDirtyFlagsEagerReadOnly<>& GetResourceDescriptorDirtyFlags() override;

	// ----------------------------------------------------
	// Resources Barriers Emitter

	public:
		std::vector<D3D12_RESOURCE_BARRIER> TransiteResourceBarriers(UtBarriersFlags const flags) override;
	};
}

// --------------------------------------------------------

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/rt_resource_descriptor_function.h>
#include <aiva/layer1_ext/rt_resource_descriptor_params.h>
#include <aiva/layer1_ext/rt_resource_state_function.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	RtResourceView<TResourceType, TResourceViewType>::RtResourceView(std::shared_ptr<ResourceClass> const& resource, ParamsClass const& params, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitGenericFields(resource, params);
	}

	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	RtResourceView<TResourceType, TResourceViewType>::~RtResourceView()
	{
		ShutGenericFields();
	}

	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	std::shared_ptr<typename RtResourceView<TResourceType, TResourceViewType>::ResourceClass> const& RtResourceView<TResourceType, TResourceViewType>::Resource()
	{
		return mResource;
	}

	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	void RtResourceView<TResourceType, TResourceViewType>::Resource(std::shared_ptr<ResourceClass> const& resource)
	{
		if (mResource)
		{
			mResource->DirtyFlags().OnAfterDirtied().RemoveListener(&ThisType::GenericFields_When_Resource_OnDirtied, this);
		}

		mResource = resource;

		if (mResource)
		{
			mResource->DirtyFlags().OnAfterDirtied().AttachListener(&ThisType::GenericFields_When_Resource_OnDirtied, this);
		}

		GenericFields_When_Resource_OnDirtied();
	}

	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	void RtResourceView<TResourceType, TResourceViewType>::InitGenericFields(std::shared_ptr<ResourceClass> const& resource, ParamsClass const& params)
	{
		Resource(resource);
		Params(params);

		aiva::utils::Asserts::CheckBool(mResource, "Resource is not valid");
	}

	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	void RtResourceView<TResourceType, TResourceViewType>::ShutGenericFields()
	{
		aiva::utils::Asserts::CheckBool(mResource, "Resource is not valid");

		Params({});
		Resource({});
	}

	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	void RtResourceView<TResourceType, TResourceViewType>::GenericFields_When_Resource_OnDirtied()
	{
		mDirtyFlags.Dirty();
	}

	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	void RtResourceView<TResourceType, TResourceViewType>::CreateResourceDescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE const& outHandle)
	{
		aiva::utils::Asserts::CheckBool(mResource, "Resource is not valid");

		auto const& nativeResource = mResource->NativeResource();
		aiva::utils::Asserts::CheckBool(nativeResource, "Native resource is not valid");

		auto const& directResource = nativeResource->Object();
		// ---------------- may be null ----------------

		auto const params = RtResourceDescriptorParams<TResourceType, TResourceViewType>{ directResource, mParams, mEngine, outHandle };
		RtResourceDescriptorFunction(params);
	}

	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	aiva::utils_ext::TDirtyFlagsEagerReadOnly<>& RtResourceView<TResourceType, TResourceViewType>::GetResourceDescriptorDirtyFlags()
	{
		return mDirtyFlags;
	}

	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	std::vector<D3D12_RESOURCE_BARRIER> RtResourceView<TResourceType, TResourceViewType>::TransiteResourceBarriers(UtBarriersFlags const flags)
	{
		auto const& resource = mResource;
		aiva::utils::Asserts::CheckBool(resource, "Resource is not valid");

		auto const& nativeResource = resource->NativeResource();
		aiva::utils::Asserts::CheckBool(nativeResource, "Native resource is not valid");

		auto resourceBarriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		auto const& directResource = nativeResource->Object();
		if (directResource)
		{
			auto const directDesc = directResource->GetDesc();
			auto const subResource = mParams.ToSubResource(directDesc.DepthOrArraySize, directDesc.MipLevels);
			auto const desiredState = RtResourceStateFunction<TResourceType, TResourceViewType>(*this, flags);

			resourceBarriers = std::move(resource->TransiteResourceBarriers(subResource, desiredState));
		}

		return resourceBarriers;
	}
}
