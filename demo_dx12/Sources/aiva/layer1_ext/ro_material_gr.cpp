#include <pch.h>
#include <aiva/layer1_ext/ro_material_gr.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1_ext/nr_texture_2d.h>
#include <aiva/layer1_ext/resource_system.h>
#include <aiva/layer1_ext/ro_shader_frag.h>
#include <aiva/layer1_ext/ro_shader_vert.h>
#include <aiva/layer1_ext/ro_texture_2d.h>
#include <aiva/layer1_ext/rt_table_main.h>
#include <aiva/layer1_ext/shader_ia_input_layout_desc.h>
#include <aiva/layer1_ext/shader_rs_root_signature.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	RoMaterialGr::RoMaterialGr(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitGenericFields();
		InitPipelineStateFields();
		InitMaterialParamsFields();
	}

	RoMaterialGr::~RoMaterialGr()
	{
		ShutMaterialParamsFields();
		ShutPipelineStateFields();
		ShutGenericFields();
	}

	void RoMaterialGr::DeserealizeFromJson(nlohmann::json const& resourceJsn)
	{
		Asserts::CheckBool(!std::empty(resourceJsn), "Resource jsn is not valid");

		{
			auto const& pipelineParams = resourceJsn.at("pipeline_params");
			VertexShader(Engine().ResourceSystemExt().GetResource<RoShaderVert>(pipelineParams.at("vertex_shader").get<std::filesystem::path>()));
			FragmentShader(Engine().ResourceSystemExt().GetResource<RoShaderFrag>(pipelineParams.at("fragment_shader").get<std::filesystem::path>()));
			FillMode(pipelineParams.at("fill_mode").get<std::remove_reference_t<decltype(FillMode())>>());
			CullMode(pipelineParams.at("cull_mode").get<std::remove_reference_t<decltype(CullMode())>>());
			DepthTest(pipelineParams.at("depth_test").get<std::remove_reference_t<decltype(DepthTest())>>());
			DepthWrite(pipelineParams.at("depth_write").get<std::remove_reference_t<decltype(DepthWrite())>>());
			DepthFunc(pipelineParams.at("depth_func").get<std::remove_reference_t<decltype(DepthFunc())>>());
			RenderTargets(pipelineParams.at("render_targets").get<std::remove_reference_t<decltype(RenderTargets())>>());
			DepthTarget(pipelineParams.at("depth_target").get<std::remove_reference_t<decltype(DepthTarget())>>());
		}

		{
			auto const& resourcesParams = resourceJsn.at("resources_params");
			for (auto const& resourceParam : resourcesParams)
			{
				auto const resource = mEngine.ResourceSystemExt().GetResource<RoTexture2D>(resourceParam.at("path").get<std::string>());
				SetResource(resourceParam.at("name").get<std::string>(), resource);
			}
		}
	}

	void RoMaterialGr::InitGenericFields()
	{
		DirtyFlagsForPipelineStateFields().OnAfterDirtied().AttachListener(&ThisType::GenericFields_When_PipelineStateFields_OnDirtied, this);
	}

	void RoMaterialGr::ShutGenericFields()
	{
		DirtyFlagsForPipelineStateFields().OnAfterDirtied().RemoveListener(&ThisType::GenericFields_When_PipelineStateFields_OnDirtied, this);
	}

	void RoMaterialGr::GenericFields_When_PipelineStateFields_OnDirtied()
	{
		DirtyFlags().Dirty();
	}

	winrt::com_ptr<ID3D12RootSignature> const& RoMaterialGr::NativeRootSignature()
	{
		mDirtyFlagsForPipelineStateFields.Flush();

		auto const& rootSignatureObject = mRootSignature;
		Asserts::CheckBool(rootSignatureObject, "Root signature object is not valid");

		auto const& rootSignatureResource = rootSignatureObject->NativeResource();
		Asserts::CheckBool(rootSignatureResource, "Root signature resource is not valid");

		return rootSignatureResource;
	}

	winrt::com_ptr<ID3D12PipelineState> const& RoMaterialGr::NativePipelineState()
	{
		mDirtyFlagsForPipelineStateFields.Flush();

		auto const& pipelineStateObject = mPipelineState;
		Asserts::CheckBool(pipelineStateObject, "Pipeline state object is not valid");

		return pipelineStateObject;
	}

	void RoMaterialGr::InitPipelineStateFields()
	{
		DirtyFlagsForPipelineParamsFields().OnAfterDirtied().AttachListener(&ThisType::PipelineStateFields_When_PipelineParamsFields_OnDirtied, this);
		DirtyFlagsForPipelineStateFields().OnBeforeFlushed().AttachListener(&ThisType::PipelineStateFields_When_PipelineStateFields_OnFlushed, this);
	}

	void RoMaterialGr::ShutPipelineStateFields()
	{
		DirtyFlagsForPipelineStateFields().OnBeforeFlushed().RemoveListener(&ThisType::PipelineStateFields_When_PipelineStateFields_OnFlushed, this);
		DirtyFlagsForPipelineParamsFields().OnAfterDirtied().RemoveListener(&ThisType::PipelineStateFields_When_PipelineParamsFields_OnDirtied, this);
	}

	void RoMaterialGr::PipelineStateFields_When_PipelineParamsFields_OnDirtied()
	{
		DirtyFlagsForPipelineStateFields().Dirty();
	}

	void RoMaterialGr::PipelineStateFields_When_PipelineStateFields_OnFlushed()
	{
		FlushRootSignature();
		FlushPipelineState();
	}

	void RoMaterialGr::FlushRootSignature()
	{
		mRootSignature = NewObject<ShaderRSRootSignature>(shared_from_this(), Engine());
		Asserts::CheckBool(mRootSignature, "Root signature is not valid");
	}

	void RoMaterialGr::FlushPipelineState()
	{
		auto const& device = Engine().GraphicHardware().Device();
		Asserts::CheckBool(device, "Device is not valid");

		auto pipelineStateDesc = D3D12_GRAPHICS_PIPELINE_STATE_DESC{};

		{ // Root Signature
			auto const& rootSignature = NativeRootSignature();
			Asserts::CheckBool(rootSignature, "Root signature is not valid");

			pipelineStateDesc.pRootSignature = rootSignature.get();
		}

		{ // Vertex Shader
			auto const& vertexShader = VertexShader();
			Asserts::CheckBool(vertexShader, "Vertex shader is not valid");

			auto const& shaderBytecode = vertexShader->Bytecode();
			Asserts::CheckBool(shaderBytecode, "Shader bytecode is not valid");

			pipelineStateDesc.VS.pShaderBytecode = shaderBytecode->GetBufferPointer();
			pipelineStateDesc.VS.BytecodeLength = shaderBytecode->GetBufferSize();
		}

		{ // Fragment Shader
			auto const& fragmentShader = FragmentShader();
			Asserts::CheckBool(fragmentShader, "Fragment shader is not valid");

			auto const& shaderBytecode = fragmentShader->Bytecode();
			Asserts::CheckBool(shaderBytecode, "Shader bytecode is not valid");

			pipelineStateDesc.PS.pShaderBytecode = shaderBytecode->GetBufferPointer();
			pipelineStateDesc.PS.BytecodeLength = shaderBytecode->GetBufferSize();
		}

		{ // Blend State
			pipelineStateDesc.BlendState.AlphaToCoverageEnable = false;
			pipelineStateDesc.BlendState.IndependentBlendEnable = false;
			pipelineStateDesc.BlendState.RenderTarget[0].BlendEnable = false;
			pipelineStateDesc.BlendState.RenderTarget[0].LogicOpEnable = false;
			pipelineStateDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
			pipelineStateDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
			pipelineStateDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			pipelineStateDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			pipelineStateDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			pipelineStateDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			pipelineStateDesc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
			pipelineStateDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}

		{ // Sample Mask
			pipelineStateDesc.SampleMask = UINT_MAX;
		}

		{ // Rasterizer State
			pipelineStateDesc.RasterizerState.FillMode = aiva::layer1::ToInternalEnum(FillMode());
			pipelineStateDesc.RasterizerState.CullMode = aiva::layer1::ToInternalEnum(CullMode());
			pipelineStateDesc.RasterizerState.FrontCounterClockwise = true;
			pipelineStateDesc.RasterizerState.DepthBias = {};
			pipelineStateDesc.RasterizerState.DepthBiasClamp = {};
			pipelineStateDesc.RasterizerState.SlopeScaledDepthBias = {};
			pipelineStateDesc.RasterizerState.DepthClipEnable = true;
			pipelineStateDesc.RasterizerState.MultisampleEnable = false;
			pipelineStateDesc.RasterizerState.AntialiasedLineEnable = false;
			pipelineStateDesc.RasterizerState.ForcedSampleCount = {};
			pipelineStateDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		}

		{ // Depth-Stencil State
			pipelineStateDesc.DepthStencilState.DepthEnable = DepthTest();
			pipelineStateDesc.DepthStencilState.DepthWriteMask = DepthWrite() ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
			pipelineStateDesc.DepthStencilState.DepthFunc = aiva::layer1::ToInternalEnum(DepthFunc());
		}

		{ // Input Layout
			auto const& vertexShader = VertexShader();
			Asserts::CheckBool(vertexShader, "Vertex shader is not valid");

			auto const& inputLayout = vertexShader->InputLayout();
			Asserts::CheckBool(inputLayout, "Input layout is not valid");

			pipelineStateDesc.InputLayout = inputLayout->NativeResource();
		}

		{ // Index Buffer Strip Cut Value
			pipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		}

		{ // Primitive Topology Type
			pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		}

		{ // Render Targets
			pipelineStateDesc.NumRenderTargets = std::min(std::size(RenderTargets()), std::size(pipelineStateDesc.RTVFormats));
			std::transform(std::cbegin(RenderTargets()), std::cend(RenderTargets()), std::begin(pipelineStateDesc.RTVFormats), [](auto const depthTarget) { return aiva::layer1::ToInternalEnum(depthTarget); });
		}

		{ // Depth-Stencil Target
			pipelineStateDesc.DSVFormat = aiva::layer1::ToInternalEnum(DepthTarget());
		}

		{ // Sample Desc
			pipelineStateDesc.SampleDesc.Count = 1;
			pipelineStateDesc.SampleDesc.Quality = 0;
		}

		{ // Node Mask
			pipelineStateDesc.NodeMask = {};
		}

		{ // Cached PSO
			pipelineStateDesc.CachedPSO = {};
		}

		{ // Flags
			pipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		}

		winrt::check_hresult(device->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(&mPipelineState)));
		Asserts::CheckBool(mPipelineState, "Pipeline state is not valid");
	}

	std::shared_ptr<RoShaderVert> const& RoMaterialGr::VertexShader()
	{
		return mVertexShader;
	}

	void RoMaterialGr::VertexShader(std::shared_ptr<RoShaderVert> const& value)
	{
		if (mVertexShader != value)
		{
			mVertexShader = value;
			DirtyFlagsForPipelineParamsFields().Dirty();
		}
	}

	std::shared_ptr<RoShaderFrag> const& RoMaterialGr::FragmentShader()
	{
		return mFragmentShader;
	}

	void RoMaterialGr::FragmentShader(std::shared_ptr<RoShaderFrag> const& value)
	{
		if (mFragmentShader != value)
		{
			mFragmentShader = value;
			DirtyFlagsForPipelineParamsFields().Dirty();
		}
	}

	aiva::layer1::EFillMode const& RoMaterialGr::FillMode()
	{
		return mFillMode;
	}

	void RoMaterialGr::FillMode(aiva::layer1::EFillMode const& value)
	{
		if (mFillMode != value)
		{
			mFillMode = value;
			DirtyFlagsForPipelineParamsFields().Dirty();
		}
	}

	aiva::layer1::ECullMode const& RoMaterialGr::CullMode()
	{
		return mCullMode;
	}

	void RoMaterialGr::CullMode(aiva::layer1::ECullMode const& value)
	{
		if (mCullMode != value)
		{
			mCullMode = value;
			DirtyFlagsForPipelineParamsFields().Dirty();
		}
	}

	bool const& RoMaterialGr::DepthTest()
	{
		return mDepthTest;
	}

	void RoMaterialGr::DepthTest(bool const& value)
	{
		if (mDepthTest != value)
		{
			mDepthTest = value;
			DirtyFlagsForPipelineParamsFields().Dirty();
		}
	}

	bool const& RoMaterialGr::DepthWrite()
	{
		return mDepthWrite;
	}

	void RoMaterialGr::DepthWrite(bool const& value)
	{
		if (mDepthWrite != value)
		{
			mDepthWrite = value;
			DirtyFlagsForPipelineParamsFields().Dirty();
		}
	}

	aiva::layer1::EComparisonFunc const& RoMaterialGr::DepthFunc()
	{
		return mDepthFunc;
	}

	void RoMaterialGr::DepthFunc(aiva::layer1::EComparisonFunc const& value)
	{
		if (mDepthFunc != value)
		{
			mDepthFunc = value;
			DirtyFlagsForPipelineParamsFields().Dirty();
		}
	}

	std::vector<aiva::layer1::EResourceBufferFormat> const& RoMaterialGr::RenderTargets()
	{
		return mRenderTargets;
	}

	void RoMaterialGr::RenderTargets(std::vector<aiva::layer1::EResourceBufferFormat> const& value)
	{
		if (mRenderTargets != value)
		{
			mRenderTargets = value;
			DirtyFlagsForPipelineParamsFields().Dirty();
		}
	}

	aiva::layer1::EResourceBufferFormat const& RoMaterialGr::DepthTarget()
	{
		return mDepthTarget;
	}

	void RoMaterialGr::DepthTarget(aiva::layer1::EResourceBufferFormat const& value)
	{
		if (mDepthTarget != value)
		{
			mDepthTarget = value;
			DirtyFlagsForPipelineParamsFields().Dirty();
		}
	}

	std::vector<winrt::com_ptr<ID3D12DescriptorHeap>> RoMaterialGr::NativeDescriptorHeaps() const
	{
		auto const& resourceTable = mResourceTable;
		Asserts::CheckBool(resourceTable, "Resource table is not valid");

		return resourceTable->NativeResource();
	}

	void RoMaterialGr::InitMaterialParamsFields()
	{
		mResourceTable = NewObject<RtTableMain>(mEngine);
		Asserts::CheckBool(mResourceTable, "Resource table is not valid");
	}

	void RoMaterialGr::ShutMaterialParamsFields()
	{
		Asserts::CheckBool(mResourceTable, "Resource table is not valid");
		mResourceTable = {};
	}

	std::vector<D3D12_RESOURCE_BARRIER> RoMaterialGr::TransiteResourceBarriers(UtBarriersFlags const flags)
	{
		auto const& resourceTable = mResourceTable;
		Asserts::CheckBool(resourceTable, "Resource table is not valid");

		return resourceTable->TransiteResourceBarriers(flags);
	}
}
