#include <pch.h>
#include <aiva/layer1_ext/shader_ia_input_layout_desc.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/ro_shader_vert.h>
#include <aiva/layer1_ext/shader_ia_input_element_desc.h>
#include <aiva/layer1_ext/shader_layout_for_field.h>
#include <aiva/layer1_ext/shader_layout_for_struct.h>
#include <aiva/layer1_ext/shader_meta_for_code.h>
#include <aiva/layer1_ext/shader_meta_for_field.h>
#include <aiva/layer1_ext/shader_meta_for_semantic.h>
#include <aiva/layer1_ext/shader_meta_for_struct.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderIAInputLayoutDesc::ShaderIAInputLayoutDesc(std::shared_ptr<RoShaderVert> const& vertexShader, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitGenericFields(vertexShader);
	}

	ShaderIAInputLayoutDesc::ShaderIAInputLayoutDesc(std::shared_ptr<ShaderMetaForCode> const& codeMeta, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitGenericFields(codeMeta);
	}

	ShaderIAInputLayoutDesc::ShaderIAInputLayoutDesc(std::shared_ptr<ShaderMetaForStruct> const& structMeta, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitGenericFields(structMeta);
	}

	ShaderIAInputLayoutDesc::~ShaderIAInputLayoutDesc()
	{

	}

	void ShaderIAInputLayoutDesc::InitGenericFields(std::shared_ptr<RoShaderVert> const& vertexShader)
	{
		Asserts::CheckBool(vertexShader, "Vertex shader is not valid");
		
		auto const& codeMeta = vertexShader->Metadata();
		Asserts::CheckBool(codeMeta, "Code meta is not valid");

		InitGenericFields(codeMeta);
	}

	void ShaderIAInputLayoutDesc::InitGenericFields(std::shared_ptr<ShaderMetaForCode> const& codeMeta)
	{
		Asserts::CheckBool(codeMeta, "Code meta is not valid");

		auto const& structMeta = codeMeta->StructureForInput();
		Asserts::CheckBool(structMeta, "Struct meta is not valid");

		InitGenericFields(structMeta);
	}

	void ShaderIAInputLayoutDesc::InitGenericFields(std::shared_ptr<ShaderMetaForStruct> const& structMeta)
	{
		Asserts::CheckBool(structMeta, "Struct meta is not valid");

		mNativeResource = {};
		mShadowForElementsData = {};
		mShadowForElementsView = {};

		auto const structLayout = NewObject<ShaderLayoutForStruct>(structMeta, std::size_t{}, ShaderPackingRulesForStruct::Tight, mEngine);
		Asserts::CheckBool(structLayout, "Struct layout is not valid");

		auto const numberOfMetaFields = std::size(structMeta->Fields());
		auto const numberOfLayoutFields = std::size(structLayout->Fields());
		Asserts::CheckBool(numberOfMetaFields == numberOfLayoutFields, "Number of fields is not valid");
		auto const numberOfFields = std::min(numberOfMetaFields, numberOfLayoutFields);

		for (auto i = std::size_t{}; i < numberOfFields; i++)
		{
			auto const& metaField = structMeta->Fields().at(i);
			auto const& layoutField = structLayout->Fields().at(i);

			Asserts::CheckBool(metaField, "Meta field is not valid");
			Asserts::CheckBool(metaField->Semantic(), "Meta field semantic is not valid");
			Asserts::CheckBool(metaField->Semantic()->IsInputAssembly() || metaField->Semantic()->IsSystemValue(), "Meta field semantic has the wrong flags");
			Asserts::CheckBool(layoutField, "Layout field is not valid");
			Asserts::CheckBool(metaField->Semantic()->IsSystemValue() == !layoutField->IsPartOfLayout(), "Fields has the wrong flags");

			if (metaField->Semantic()->IsSystemValue() || !layoutField->IsPartOfLayout())
			{
				continue;
			}

			auto& data = mShadowForElementsData.emplace_back();
			auto& view = mShadowForElementsView.emplace_back(NewObject<ShaderIAInputElementDesc>(metaField, layoutField, data, Engine()));
			Asserts::CheckBool(view, "View is not valid");
		}

		mNativeResource.pInputElementDescs = std::data(mShadowForElementsData);
		mNativeResource.NumElements = std::size(mShadowForElementsData);
	}
}
