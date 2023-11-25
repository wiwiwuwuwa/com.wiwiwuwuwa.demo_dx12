#include <pch.h>
#include <aiva/layer1_ext/shader_rs_root_signature_desc.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/e_shader_register_type_utils.h>
#include <aiva/layer1_ext/e_shader_root_descriptor_table_type.h>
#include <aiva/layer1_ext/e_shader_root_descriptor_table_type_utils.h>
#include <aiva/layer1_ext/ro_material_gr.h>
#include <aiva/layer1_ext/ro_shader_frag.h>
#include <aiva/layer1_ext/ro_shader_vert.h>
#include <aiva/layer1_ext/shader_meta_for_code.h>
#include <aiva/layer1_ext/shader_meta_for_register.h>
#include <aiva/layer1_ext/shader_meta_for_resource.h>
#include <aiva/layer1_ext/shader_rs_root_descriptor_table.h>
#include <aiva/layer1_ext/shader_struct_register_info.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/enum_utils.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderRSRootSignatureDesc::ShaderRSRootSignatureDesc(std::shared_ptr<RoMaterialGr> const& graphicMaterial, aiva::layer1::Engine& engine) : ShaderRSRootSignatureDesc{ ExtractRegisters(graphicMaterial), engine }
	{

	}

	ShaderRSRootSignatureDesc::ShaderRSRootSignatureDesc(std::vector<ShaderStructRegisterInfo> const& registersInfo, aiva::layer1::Engine& engine) : ShaderRSRootSignatureDesc{ ShaderMetaForRegister::Convert(registersInfo, engine), engine }
	{

	}

	ShaderRSRootSignatureDesc::ShaderRSRootSignatureDesc(std::vector<std::shared_ptr<ShaderMetaForRegister>> const& registersMeta, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		Asserts::CheckBool(!std::empty(registersMeta), "Registers meta are not valid");

		auto registersForEachTable = std::map<EShaderRootDescriptorTableType, std::vector<std::shared_ptr<ShaderMetaForRegister>>>{};

		for (auto const& registerMeta : registersMeta)
		{
			Asserts::CheckBool(registerMeta, "Register meta is not valid");

			auto const& registerType = registerMeta->Value();
			Asserts::CheckBool(EShaderRegisterTypeUtils::IsValid(registerType), "Register type is not valid");

			auto const tableType = EShaderRootDescriptorTableTypeUtils::Parse(registerType);
			Asserts::CheckBool(EShaderRootDescriptorTableTypeUtils::IsValid(tableType), "Table type is not valid");

			registersForEachTable[tableType].emplace_back(registerMeta);
		}

		for (auto const& pair : registersForEachTable)
		{
			auto const& registersForSpecificTable = pair.second;
			if (std::empty(registersForSpecificTable))
			{
				continue;
			}

			auto& data = mShadowForElementsData.emplace_back();
			auto& view = mShadowForElementsView.emplace_back(NewObject<ShaderRSRootDescriptorTable>(registersForSpecificTable, data, engine));
			Asserts::CheckBool(view, "View is not valid");
		}

		mNativeResource.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
		mNativeResource.Desc_1_1.NumParameters = static_cast<decltype(mNativeResource.Desc_1_1.NumParameters)>(std::size(mShadowForElementsData));
		mNativeResource.Desc_1_1.pParameters = std::data(mShadowForElementsData);
		mNativeResource.Desc_1_1.NumStaticSamplers = {};
		mNativeResource.Desc_1_1.pStaticSamplers = {};
		mNativeResource.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	}

	ShaderRSRootSignatureDesc::~ShaderRSRootSignatureDesc()
	{

	}

	std::vector<ShaderStructRegisterInfo> ShaderRSRootSignatureDesc::ExtractRegisters(std::shared_ptr<RoMaterialGr> const& graphicMaterial)
	{
		Asserts::CheckBool(graphicMaterial, "Graphic material is not valid");

		auto totalRegisters = std::set<ShaderStructRegisterInfo>{};

		{
			auto const& vertexShader = graphicMaterial->VertexShader();
			Asserts::CheckBool(vertexShader, "Vertex shader is not valid");

			auto const vertexRegisters = ExtractRegisters(vertexShader);
			totalRegisters.insert(std::cbegin(vertexRegisters), std::cend(vertexRegisters));
		}

		{
			auto const& fragmentShader = graphicMaterial->FragmentShader();
			Asserts::CheckBool(fragmentShader, "Fragment shader is not valid");

			auto const fragmentRegisters = ExtractRegisters(fragmentShader);
			totalRegisters.insert(std::cbegin(fragmentRegisters), std::cend(fragmentRegisters));
		}

		return std::vector<ShaderStructRegisterInfo>{ std::cbegin(totalRegisters), std::cend(totalRegisters ) };
	}

	std::vector<ShaderStructRegisterInfo> ShaderRSRootSignatureDesc::ExtractRegisters(std::shared_ptr<RoShaderFrag> const& fragShader)
	{
		Asserts::CheckBool(fragShader, "Frag shader is not valid");
		return ExtractRegisters(fragShader->Metadata());
	}

	std::vector<ShaderStructRegisterInfo> ShaderRSRootSignatureDesc::ExtractRegisters(std::shared_ptr<RoShaderVert> const& vertShader)
	{
		Asserts::CheckBool(vertShader, "Vert shader is not valid");
		return ExtractRegisters(vertShader->Metadata());
	}

	std::vector<ShaderStructRegisterInfo> ShaderRSRootSignatureDesc::ExtractRegisters(std::shared_ptr<ShaderMetaForCode> const& codeMeta)
	{
		Asserts::CheckBool(codeMeta, "Code meta is not valid");

		auto registersInfo = std::vector<ShaderStructRegisterInfo>{};

		for (auto const& resourceMeta : codeMeta->Resources())
		{
			Asserts::CheckBool(resourceMeta, "Resource meta is not valid");

			auto const& registerMeta = resourceMeta->Register();
			Asserts::CheckBool(registerMeta, "Register meta is not valid");

			registersInfo.emplace_back(registerMeta);
		}

		return registersInfo;
	}
}
