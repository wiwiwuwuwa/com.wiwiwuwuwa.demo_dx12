#include <pch.h>
#include <aiva/layer1_ext/shader_rs_root_descriptor_table.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/shader_meta_for_register.h>
#include <aiva/layer1_ext/shader_rs_descriptor_range.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderRSRootDescriptorTable::ShaderRSRootDescriptorTable(std::vector<std::shared_ptr<ShaderMetaForRegister>> const& registersMeta, D3D12_ROOT_PARAMETER1& nativeResource, aiva::layer1::Engine& engine) : mEngine{ engine }, mNativeResource{ nativeResource }
	{
		Asserts::CheckBool(!std::empty(registersMeta), "Registers meta is not valid");

		for (auto const& registerMeta : registersMeta)
		{
			Asserts::CheckBool(registerMeta, "Register meta is not valid");

			auto& data = mShadowForElementsData.emplace_back();
			auto& view = mShadowForElementsView.emplace_back(NewObject<ShaderRSDescriptorRange>(registerMeta, data, Engine()));
			Asserts::CheckBool(view, "View is not valid");
		}

		mNativeResource.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		mNativeResource.DescriptorTable.pDescriptorRanges = std::data(mShadowForElementsData);
		mNativeResource.DescriptorTable.NumDescriptorRanges = std::size(mShadowForElementsData);
		mNativeResource.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	}

	ShaderRSRootDescriptorTable::~ShaderRSRootDescriptorTable()
	{

	}
}
