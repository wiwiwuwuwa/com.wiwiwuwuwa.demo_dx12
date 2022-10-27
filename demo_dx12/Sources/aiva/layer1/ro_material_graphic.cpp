#include <pch.h>
#include <aiva/layer1/ro_material_graphic.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/material_pipeline_descriptor.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/ro_shader_fragment.h>
#include <aiva/layer1/ro_shader_vertex.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	RoMaterialGraphic::RoMaterialGraphic(EngineType const& engine)
		: AObject{}, IObjectCacheable{ true }, IObjectEngineable{ engine }, ICpuResource{}
	{
		InitializePipelineDescriptor();
		InitializeResourceDescriptor();
		InitializeInternalPipelineState();
	}

	RoMaterialGraphic::~RoMaterialGraphic()
	{
		TerminateInternalPipelineState();
		TerminateResourceDescriptor();
		TerminatePipelineDescriptor();
	}

	void RoMaterialGraphic::DeserealizeFromBinary(std::vector<std::byte> const& binaryData)
	{
		Asserts::CheckBool(!binaryData.empty());

		auto root = nlohmann::json::parse(binaryData);
		{
			auto const vertexShader = root.at("vertex_shader").get<std::string>();
			VertexShader(Engine().ResourceSystem().GetResource<RoShaderVertex>(vertexShader));

			auto const fragmentShader = root.at("fragment_shader").get<std::string>();
			FragmentShader(Engine().ResourceSystem().GetResource<RoShaderFragment>(fragmentShader));

			auto const& pipelineDescriptor = root.at("pipeline_descriptor");
			{
				auto const fillMode = pipelineDescriptor.at("fill_mode").get<EFillMode>();
				PipelineDescriptor().FillMode(fillMode);

				auto const cullMode = pipelineDescriptor.at("cull_mode").get<ECullMode>();
				PipelineDescriptor().CullMode(cullMode);

				auto const depthTest = pipelineDescriptor.at("depth_test").get<bool>();
				PipelineDescriptor().DepthTest(depthTest);

				auto const depthWrite = pipelineDescriptor.at("depth_write").get<bool>();
				PipelineDescriptor().DepthWrite(depthWrite);

				auto const depthFunc = pipelineDescriptor.at("depth_func").get<EComparisonFunc>();
				PipelineDescriptor().DepthFunc(depthFunc);

				{
					auto const& rtJsons = pipelineDescriptor.at("render_targets");

					auto rtEnums = std::vector<EResourceBufferFormat>{};
					std::transform(std::cbegin(rtJsons), std::cend(rtJsons), std::back_inserter(rtEnums), [](auto const& rtJson) { return rtJson.get<EResourceBufferFormat>(); });

					PipelineDescriptor().RenderTargets(rtEnums);
				}

				auto const depthTarget = pipelineDescriptor.at("depth_target").get<EResourceBufferFormat>();
				PipelineDescriptor().DepthTarget(depthTarget);
			}
		}

		MarkCacheDataAsChanged();
	}

	std::shared_ptr<RoShaderVertex> const& RoMaterialGraphic::VertexShader() const
	{
		return mVertexShader;
	}

	RoMaterialGraphic& RoMaterialGraphic::VertexShader(std::shared_ptr<RoShaderVertex> const& vertexShader)
	{
		if (mVertexShader != vertexShader)
		{
			mVertexShader = vertexShader;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	std::shared_ptr<RoShaderFragment> const& RoMaterialGraphic::FragmentShader() const
	{
		return mFragmentShader;
	}

	RoMaterialGraphic& RoMaterialGraphic::FragmentShader(std::shared_ptr<RoShaderFragment> const& fragmentShader)
	{
		if (mFragmentShader != fragmentShader)
		{
			mFragmentShader = fragmentShader;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	MaterialPipelineDescriptor& RoMaterialGraphic::PipelineDescriptor() const
	{
		Asserts::CheckBool(mPipelineDescriptor);
		return *mPipelineDescriptor;
	}

	void RoMaterialGraphic::InitializePipelineDescriptor()
	{
		mPipelineDescriptor = decltype(mPipelineDescriptor)::element_type::Create(Engine());
		Asserts::CheckBool(mPipelineDescriptor);

		mPipelineDescriptor->CacheUpdater().OnMarkAsChanged().connect(boost::bind(&RoMaterialGraphic::OnPipelineDescriptorMarkedAsChanged, this));
	}

	void RoMaterialGraphic::TerminatePipelineDescriptor()
	{
		Asserts::CheckBool(mPipelineDescriptor);

		mPipelineDescriptor->CacheUpdater().OnMarkAsChanged().disconnect(boost::bind(&RoMaterialGraphic::OnPipelineDescriptorMarkedAsChanged, this));
		mPipelineDescriptor = {};
	}

	void RoMaterialGraphic::OnPipelineDescriptorMarkedAsChanged()
	{
		MarkCacheDataAsChanged();
	}

	MaterialResourceDescriptor& RoMaterialGraphic::ResourceDescriptor() const
	{
		Asserts::CheckBool(mResourceDescriptor);
		return *mResourceDescriptor;
	}

	void RoMaterialGraphic::InitializeResourceDescriptor()
	{
		mResourceDescriptor = NewObject<decltype(mResourceDescriptor)::element_type>(Engine());
		Asserts::CheckBool(mResourceDescriptor);

		mResourceDescriptor->OnMarkCacheDataAsChanged().connect(boost::bind(&RoMaterialGraphic::OnResourceDescriptorMarkedAsChanged, this));
	}

	void RoMaterialGraphic::TerminateResourceDescriptor()
	{
		Asserts::CheckBool(mResourceDescriptor);

		mResourceDescriptor->OnMarkCacheDataAsChanged().disconnect(boost::bind(&RoMaterialGraphic::OnResourceDescriptorMarkedAsChanged, this));
		mResourceDescriptor = {};
	}

	void RoMaterialGraphic::OnResourceDescriptorMarkedAsChanged()
	{
		MarkCacheDataAsChanged();
	}

	winrt::com_ptr<ID3D12PipelineState> const& RoMaterialGraphic::InternalPipelineState()
	{
		FlushCacheDataChanges();

		winrt::check_bool(mInternalPipelineState);
		return mInternalPipelineState;
	}

	void RoMaterialGraphic::InitializeInternalPipelineState()
	{
		FlushCacheDataExecutors().connect(boost::bind(&RoMaterialGraphic::RefreshInternalPipelineState, this));
	}

	void RoMaterialGraphic::TerminateInternalPipelineState()
	{
		FlushCacheDataExecutors().disconnect(boost::bind(&RoMaterialGraphic::RefreshInternalPipelineState, this));
	}

	void RoMaterialGraphic::RefreshInternalPipelineState()
	{
		auto const& device = Engine().GraphicHardware().Device();
		winrt::check_bool(device);

		auto pipelineDesc = D3D12_GRAPHICS_PIPELINE_STATE_DESC{};

		{ // Root Signature
			auto const& rootSignature = ResourceDescriptor().InternalRootSignature();
			pipelineDesc.pRootSignature = rootSignature.get();
		}

		{ // Vertex Shader
			auto const& vertexShader = VertexShader();
			Asserts::CheckBool(vertexShader);

			auto const& shaderBytecode = vertexShader->Bytecode();
			winrt::check_bool(shaderBytecode);

			pipelineDesc.VS.pShaderBytecode = shaderBytecode->GetBufferPointer();
			pipelineDesc.VS.BytecodeLength = shaderBytecode->GetBufferSize();
		}

		{ // Fragment Shader
			auto const& fragmentShader = FragmentShader();
			Asserts::CheckBool(fragmentShader);

			auto const& shaderBytecode = fragmentShader->Bytecode();
			winrt::check_bool(shaderBytecode);

			pipelineDesc.PS.pShaderBytecode = shaderBytecode->GetBufferPointer();
			pipelineDesc.PS.BytecodeLength = shaderBytecode->GetBufferSize();
		}

		{ // Blend State
			std::for_each(std::begin(pipelineDesc.BlendState.RenderTarget), std::end(pipelineDesc.BlendState.RenderTarget), [](auto& rt)
			{
				rt.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			});
		}

		{ // Sample Mask
			pipelineDesc.SampleMask = UINT_MAX;
		}

		{ // Rasterizer State
			auto const& descriptor = PipelineDescriptor();

			pipelineDesc.RasterizerState.FillMode = ToInternalEnum(descriptor.FillMode());
			pipelineDesc.RasterizerState.CullMode = ToInternalEnum(descriptor.CullMode());
			pipelineDesc.RasterizerState.FrontCounterClockwise = true;
			pipelineDesc.RasterizerState.DepthClipEnable = true;
		}

		{ // Depth-Stencil State
			auto const& descriptor = PipelineDescriptor();

			pipelineDesc.DepthStencilState.DepthEnable = descriptor.DepthTest();
			pipelineDesc.DepthStencilState.DepthWriteMask = descriptor.DepthWrite() ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
			pipelineDesc.DepthStencilState.DepthFunc = ToInternalEnum(descriptor.DepthFunc());
		}

		{ // Primitive Topology Type
			pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		}

		{ // Render Targets
			auto const& descriptor = PipelineDescriptor();
			auto const& renderTargets = descriptor.RenderTargets();

			pipelineDesc.NumRenderTargets = std::min(std::size(renderTargets), std::size(pipelineDesc.RTVFormats));
			std::transform(std::cbegin(renderTargets), std::cend(renderTargets), std::begin(pipelineDesc.RTVFormats), [](auto const& depthTarget) { return ToInternalEnum(depthTarget); });
		}

		{ // Depth-Stencil Target
			auto const& descriptor = PipelineDescriptor();

			pipelineDesc.DSVFormat = ToInternalEnum(descriptor.DepthTarget());
		}

		{ // Sample Desc
			pipelineDesc.SampleDesc.Count = 1;
			pipelineDesc.SampleDesc.Quality = 0;
		}

		auto pipelineState = winrt::com_ptr<ID3D12PipelineState>{};
		winrt::check_hresult(device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState)));

		winrt::check_bool(pipelineState);
		mInternalPipelineState = pipelineState;
	}

	std::vector<D3D12_RESOURCE_BARRIER> RoMaterialGraphic::PrepareBarriers(bool const active)
	{
		FlushCacheDataChanges();
		return ResourceDescriptor().PrepareBarriers(active);
	}

	RoMaterialGraphic& RoMaterialGraphic::CopyPropertiesFrom(RoMaterialGraphic const& source)
	{
		VertexShader(source.VertexShader());
		FragmentShader(source.FragmentShader());
		PipelineDescriptor().CopyPropertiesFrom(source.PipelineDescriptor());
		ResourceDescriptor().CopyPropertiesFrom(source.ResourceDescriptor());

		MarkCacheDataAsChanged();
		return *this;
	}

	std::shared_ptr<RoMaterialGraphic> RoMaterialGraphic::Copy() const
	{
		auto const copy = NewObject<RoMaterialGraphic>(Engine());
		Asserts::CheckBool(copy);

		copy->CopyPropertiesFrom(*this);
		return copy;
	}
}
