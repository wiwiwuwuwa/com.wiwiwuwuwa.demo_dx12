#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/rt_find_view_params_a.h>
#include <aiva/layer1_ext/rt_resource_type.h>
#include <aiva/layer1_ext/rt_resource_view_type.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_single_arg.h>

namespace aiva::layer1_ext
{
	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	struct RtResourceDescriptorParams final
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_FIELD_REF_3(public, M_SINGLE_ARG(winrt::com_ptr<ID3D12Resource> const), InResource);

		M_OBJECT_FIELD_REF_3(public, M_SINGLE_ARG(RtFindViewParamsAT<TResourceType, TResourceViewType> const), InParams);

		M_OBJECT_FIELD_REF_3(public, M_SINGLE_ARG(aiva::layer1::Engine const), RefEngine);

		M_OBJECT_FIELD_REF_3(public, M_SINGLE_ARG(D3D12_CPU_DESCRIPTOR_HANDLE const), OutHandle);

	public:
		RtResourceDescriptorParams(decltype(mInResource) inResource, decltype(mInParams) inParams, decltype(mRefEngine) refEngine, decltype(mOutHandle) outHandle)
			: mInResource{ inResource }, mInParams{ inParams }, mRefEngine{ refEngine }, mOutHandle{ outHandle } {}
	};
}
