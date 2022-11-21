#include <pch.h>
#include <aiva/layer1_ext/rt_resource_state_function.h>

#include <aiva/layer1_ext/rt_resource_view.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/enum_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	D3D12_RESOURCE_STATES RtResourceStateFunction<RtResourceType::Texture2D, RtResourceViewType::Srv>(RtResourceView<RtResourceType::Texture2D, RtResourceViewType::Srv> const&, UtBarriersFlags const flags)
	{
		if (EnumUtils::Has(flags, UtBarriersFlags::Setup))
		{
			return D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE;
		}

		if (EnumUtils::Has(flags, UtBarriersFlags::Reset))
		{
			return D3D12_RESOURCE_STATE_COMMON;
		}

		Asserts::CheckBool(false, "Flags are not valid");
		return {};
	}

	D3D12_RESOURCE_STATES RtResourceStateFunction<RtResourceType::Texture2D, RtResourceViewType::Rtv>(RtResourceView<RtResourceType::Texture2D, RtResourceViewType::Rtv> const&, UtBarriersFlags const flags)
	{
		if (EnumUtils::Has(flags, UtBarriersFlags::Setup))
		{
			return D3D12_RESOURCE_STATE_RENDER_TARGET;
		}

		if (EnumUtils::Has(flags, UtBarriersFlags::Reset))
		{
			return D3D12_RESOURCE_STATE_COMMON;
		}

		Asserts::CheckBool(false, "Flags are not valid");
		return {};
	}
}
