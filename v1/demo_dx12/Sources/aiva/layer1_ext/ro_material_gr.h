#pragma once
#include <pch.h>

#include <aiva/layer1/e_comparison_func.h>
#include <aiva/layer1/e_cull_mode.h>
#include <aiva/layer1/e_fill_mode.h>
#include <aiva/layer1/e_resource_buffer_format.h>
#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/ro_material_gr_fwd.h>
#include <aiva/layer1_ext/ro_shader_frag_fwd.h>
#include <aiva/layer1_ext/ro_shader_vert_fwd.h>
#include <aiva/layer1_ext/rt_table_main_fwd.h>
#include <aiva/layer1_ext/shader_rs_root_signature_fwd.h>
#include <aiva/layer1_ext/ut_resources_barriers_emitter.h>
#include <aiva/utils_ext/i_serializable_json.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_val_dirty_flags_eager.h>
#include <aiva/utils_ext/m_object_field_val_dirty_flags_lazy.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/m_object_field_var_custom.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct RoMaterialGr final : public aiva::utils_ext::TObject<RoMaterialGr>, public aiva::utils_ext::ISerializableJson, public UtResourcesBarriersEmitter
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(RoMaterialGr);

	protected:
		RoMaterialGr(aiva::layer1::Engine& engine);

	public:
		~RoMaterialGr() override;

	// ----------------------------------------------------
	// Serialization

	public:
		void DeserealizeFromJson(nlohmann::json const& resourceJsn) override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER();

	private:
		void InitGenericFields();

		void ShutGenericFields();

	private:
		void GenericFields_When_PipelineStateFields_OnDirtied();

	// ----------------------------------------------------
	// Pipeline State Fields

	public:
		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_LAZY_2(DirtyFlagsForPipelineStateFields, private);

		M_OBJECT_FIELD_VAR_3(private, std::shared_ptr<ShaderRSRootSignature>, RootSignature);

		M_OBJECT_FIELD_VAR_3(private, winrt::com_ptr<ID3D12PipelineState>, PipelineState);

	public:
		winrt::com_ptr<ID3D12RootSignature> const& NativeRootSignature();

		winrt::com_ptr<ID3D12PipelineState> const& NativePipelineState();

	private:
		void InitPipelineStateFields();

		void ShutPipelineStateFields();

	private:
		void PipelineStateFields_When_PipelineParamsFields_OnDirtied();

		void PipelineStateFields_When_PipelineStateFields_OnFlushed();

	private:
		void FlushRootSignature();

		void FlushPipelineState();

	// ----------------------------------------------------
	// Pipeline Params Fields

	public:
		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER_2(DirtyFlagsForPipelineParamsFields, private);

		M_OBJECT_FIELD_VAR_CUSTOM_4(std::shared_ptr<RoShaderVert>, VertexShader, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(std::shared_ptr<RoShaderFrag>, FragmentShader, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(aiva::layer1::EFillMode, FillMode, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(aiva::layer1::ECullMode, CullMode, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(bool, DepthTest, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(bool, DepthWrite, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(aiva::layer1::EComparisonFunc, DepthFunc, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(std::vector<aiva::layer1::EResourceBufferFormat>, RenderTargets, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(aiva::layer1::EResourceBufferFormat, DepthTarget, public, public);

	// ----------------------------------------------------
	// Material Params Fields

	public:
		M_OBJECT_FIELD_VAR_3(private, std::shared_ptr<RtTableMain>, ResourceTable);

	public:
		std::vector<winrt::com_ptr<ID3D12DescriptorHeap>> NativeDescriptorHeaps() const;

	public:
		template <typename TConstantClass>
		void SetConstant(std::string const& name, TConstantClass const& value);

		template <typename TResourceClass>
		void SetResource(std::string const& name, std::shared_ptr<TResourceClass> const& value);

	private:
		void InitMaterialParamsFields();

		void ShutMaterialParamsFields();

	// ----------------------------------------------------
	// Resources Barriers Emitter

	public:
		std::vector<D3D12_RESOURCE_BARRIER> TransiteResourceBarriers(UtBarriersFlags const flags) override;
	};
}

// --------------------------------------------------------

#include <aiva/layer1_ext/ro_shader_vert.h>
#include <aiva/layer1_ext/rt_find_resource_type.h>
#include <aiva/layer1_ext/rt_find_view_layout_b.h>
#include <aiva/layer1_ext/rt_find_view_params_a.h>
#include <aiva/layer1_ext/rt_resource_view_type.h>
#include <aiva/layer1_ext/shader_meta_for_code.h>
#include <aiva/layer1_ext/shader_meta_for_register.h>
#include <aiva/layer1_ext/shader_meta_for_resource.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	template <typename TConstantClass>
	void RoMaterialGr::SetConstant(std::string const& name, TConstantClass const& value)
	{
		aiva::utils::Asserts::CheckBool(false, "TODO: IMPLEMENT ME");
	}

	template <typename TResourceClass>
	void RoMaterialGr::SetResource(std::string const& name, std::shared_ptr<TResourceClass> const& resource)
	{
		aiva::utils::Asserts::CheckBool(!std::empty(name), "Name is not valid");
		aiva::utils::Asserts::CheckBool(resource, "Resource is not valid");

		constexpr const auto ResourceType = RtFindResourceTypeV<TResourceClass>;
		constexpr const auto ResourceViewType = RtResourceViewType::Srv;
		using LayoutType = RtFindViewLayoutBT<ResourceViewType>;
		using ParamsType = RtFindViewParamsAT<ResourceType, ResourceViewType>;

		auto const& vertexShader = VertexShader();
		aiva::utils::Asserts::CheckBool(vertexShader, "Vertex shader is not valid");

		auto const& metaForCode = vertexShader->Metadata();
		aiva::utils::Asserts::CheckBool(metaForCode, "Meta for code is not valid");

		auto const iterForResource = std::find_if(std::cbegin(metaForCode->Resources()), std::cend(metaForCode->Resources()), [&](auto const& res) { return res->Name() == name; });
		aiva::utils::Asserts::CheckBool(iterForResource != std::cend(metaForCode->Resources()), "Iter for resource is not valid");

		auto const& metaForResource = (*iterForResource);
		aiva::utils::Asserts::CheckBool(metaForResource, "Meta for resource is not valid");

		auto const& metaForRegister = metaForResource->Register();
		aiva::utils::Asserts::CheckBool(metaForRegister, "Meta for register is not valid");

		auto resourceLayout = LayoutType{};
		resourceLayout.Index(metaForRegister->Index());
		resourceLayout.Space(metaForRegister->Space());

		auto resourceParams = ParamsType{};

		auto const resourceTable = mResourceTable;
		aiva::utils::Asserts::CheckBool(resourceTable, "Resource table is not valid");

		resourceTable->InsertResource<ResourceViewType>(resource, resourceLayout, resourceParams);
	}
}
