#include <pch.h>
#include <aiva/layer1_ext/shader_rs_descriptor_range.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/dx_descriptor_range_type_utils.h>
#include <aiva/layer1_ext/e_shader_register_type_utils.h>
#include <aiva/layer1_ext/shader_meta_for_register.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	ShaderRSDescriptorRange::ShaderRSDescriptorRange(std::shared_ptr<ShaderMetaForRegister> const& registerMeta, D3D12_DESCRIPTOR_RANGE1& nativeResource, aiva::layer1::Engine& engine) : mEngine{ engine }, mNativeResource{ nativeResource }
	{
		Asserts::CheckBool(registerMeta, "Register meta is not valid");
		Asserts::CheckBool(EShaderRegisterTypeUtils::IsValid(registerMeta->Value()), "Register meta value is not valid");

		mNativeResource.RangeType = DxDescriptorRangeTypeUtils::Parse(registerMeta->Value());
		mNativeResource.NumDescriptors = 1;
		mNativeResource.BaseShaderRegister = static_cast<decltype(mNativeResource.BaseShaderRegister)>(registerMeta->Index());
		mNativeResource.RegisterSpace = static_cast<decltype(mNativeResource.RegisterSpace)>(registerMeta->Space());
		mNativeResource.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
		mNativeResource.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	ShaderRSDescriptorRange::~ShaderRSDescriptorRange()
	{

	}
}
