#include <pch.h>
#include <aiva/layer1_ext/shader_ia_input_element_desc.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/dx_dxgi_format_utils.h>
#include <aiva/layer1_ext/shader_layout_for_field.h>
#include <aiva/layer1_ext/shader_meta_for_field.h>
#include <aiva/layer1_ext/shader_meta_for_semantic.h>
#include <aiva/layer1_ext/shader_meta_for_type.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/e_value_type_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderIAInputElementDesc::ShaderIAInputElementDesc(std::shared_ptr<ShaderMetaForField> const& fieldMeta, std::shared_ptr<ShaderLayoutForField> const& fieldLayout, D3D12_INPUT_ELEMENT_DESC& nativeResource, aiva::layer1::Engine& engine) : mEngine{ engine }, mNativeResource{ nativeResource }
	{
		Asserts::CheckBool(fieldMeta, "Field meta is not valid");
		Asserts::CheckBool(fieldMeta->Semantic(), "Field meta semantic is not valid");
		Asserts::CheckBool(fieldMeta->Semantic()->IsInputAssembly(), "Field meta semantic is not an input assembly");
		Asserts::CheckBool(!fieldMeta->Semantic()->IsSystemValue(), "Field meta semantic is a system value");
		Asserts::CheckBool(fieldMeta->Type(), "Field meta type is not valid");
		Asserts::CheckBool(EValueTypeUtils::IsValid(fieldMeta->Type()->Name()), "Field meta type name is not valid");
		Asserts::CheckBool(fieldLayout, "Field layout is not valid");
		Asserts::CheckBool(fieldLayout->IsPartOfLayout(), "Field layout is not a part of a layout");

		mShadowForSemanticName = fieldMeta->Semantic()->Value();

		mNativeResource.SemanticName = mShadowForSemanticName.c_str();
		mNativeResource.SemanticIndex = fieldMeta->Semantic()->Index();
		mNativeResource.Format = DxDXGIFormatUtils::ParseFromValueType(fieldMeta->Type()->Name());
		mNativeResource.InputSlot = {};
		mNativeResource.AlignedByteOffset = fieldLayout->Offset();
		mNativeResource.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		mNativeResource.InstanceDataStepRate = {};
	}

	ShaderIAInputElementDesc::~ShaderIAInputElementDesc()
	{

	}
}
