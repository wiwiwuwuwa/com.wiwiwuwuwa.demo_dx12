#include <pch.h>
#include <aiva/layer1/ro_material_graphic.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/material_pipeline_descriptor.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/ro_shader_fragment.h>
#include <aiva/layer1/ro_shader_vertex.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::RoMaterialGraphic::RoMaterialGraphic(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
	InitializePipelineDescriptor();
	InitializeResourceDescriptor();
	InitializeInternalResources();
}

aiva::layer1::RoMaterialGraphic::~RoMaterialGraphic()
{
	TerminateInternalResources();
	TerminateResourceDescriptor();
	TerminatePipelineDescriptor();
	TerminateCacheUpdater();
}

void aiva::layer1::RoMaterialGraphic::DeserealizeFromBinary(std::vector<std::byte> const& binaryData)
{
	aiva::utils::Asserts::CheckBool(!binaryData.empty());

	auto root = nlohmann::json::parse(binaryData);
	{
		auto const& vertexShader = root.at("vertex_shader").get<std::string>();
		VertexShader(mEngine.ResourceSystem().GetResource<aiva::layer1::RoShaderVertex>(vertexShader));

		auto const& fragmentShader = root.at("fragment_shader").get<std::string>();
		FragmentShader(mEngine.ResourceSystem().GetResource<aiva::layer1::RoShaderFragment>(fragmentShader));

		auto const& pipelineDescriptor = root.at("pipeline_descriptor");
		{
			auto const& fillMode = pipelineDescriptor.at("fill_mode").get<EGpuFillMode>();
			PipelineDescriptor().FillMode(fillMode);

			auto const& cullMode = pipelineDescriptor.at("cull_mode").get<EGpuCullMode>();
			PipelineDescriptor().CullMode(cullMode);

			auto const& depthTest = pipelineDescriptor.at("depth_test").get<bool>();
			PipelineDescriptor().DepthTest(depthTest);

			auto const& depthWrite = pipelineDescriptor.at("depth_write").get<bool>();
			PipelineDescriptor().DepthWrite(depthWrite);

			auto const& depthFunc = pipelineDescriptor.at("depth_func").get<EGpuComparisonFunc>();
			PipelineDescriptor().DepthFunc(depthFunc);

			{
				auto const& rtJsons = pipelineDescriptor.at("render_targets");

				auto rtEnums = std::vector<EGpuResourceBufferFormat>{};
				std::transform(std::cbegin(rtJsons), std::cend(rtJsons), std::back_inserter(rtEnums), [](auto const& rtJson) { return rtJson.get<EGpuResourceBufferFormat>(); });

				PipelineDescriptor().RenderTargets(rtEnums);
			}

			auto const& depthTarget = pipelineDescriptor.at("depth_target").get<EGpuResourceBufferFormat>();
			PipelineDescriptor().DepthTarget(depthTarget);
		}
	}

	CacheUpdater().MarkAsChanged();
}

aiva::layer1::RoMaterialGraphic::CacheUpdaterType& aiva::layer1::RoMaterialGraphic::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::RoMaterialGraphic::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::RoMaterialGraphic::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

std::shared_ptr<aiva::layer1::RoShaderVertex> aiva::layer1::RoMaterialGraphic::VertexShader() const
{
	return mVertexShader;
}

aiva::layer1::RoMaterialGraphic& aiva::layer1::RoMaterialGraphic::VertexShader(std::shared_ptr<RoShaderVertex> const& vertexShader)
{
	mVertexShader = vertexShader;
	CacheUpdater().MarkAsChanged();

	return *this;
}

std::shared_ptr<aiva::layer1::RoShaderFragment> aiva::layer1::RoMaterialGraphic::FragmentShader() const
{
	return mFragmentShader;
}

aiva::layer1::RoMaterialGraphic& aiva::layer1::RoMaterialGraphic::FragmentShader(std::shared_ptr<RoShaderFragment> const& fragmentShader)
{
	mFragmentShader = fragmentShader;
	CacheUpdater().MarkAsChanged();

	return *this;
}

aiva::layer1::MaterialPipelineDescriptor& aiva::layer1::RoMaterialGraphic::PipelineDescriptor() const
{
	aiva::utils::Asserts::CheckBool(mPipelineDescriptor);
	return *mPipelineDescriptor;
}

void aiva::layer1::RoMaterialGraphic::InitializePipelineDescriptor()
{
	mPipelineDescriptor = decltype(mPipelineDescriptor)::element_type::Create(mEngine);
	aiva::utils::Asserts::CheckBool(mPipelineDescriptor);

	mPipelineDescriptor->CacheUpdater().OnMarkAsChanged().connect(boost::bind(&RoMaterialGraphic::OnPipelineDescriptorMarkedAsChanged, this));
}

void aiva::layer1::RoMaterialGraphic::TerminatePipelineDescriptor()
{
	aiva::utils::Asserts::CheckBool(mPipelineDescriptor);

	mPipelineDescriptor->CacheUpdater().OnMarkAsChanged().disconnect(boost::bind(&RoMaterialGraphic::OnPipelineDescriptorMarkedAsChanged, this));
	mPipelineDescriptor = {};
}

void aiva::layer1::RoMaterialGraphic::OnPipelineDescriptorMarkedAsChanged()
{
	CacheUpdater().MarkAsChanged();
}

aiva::layer1::MaterialResourceDescriptor& aiva::layer1::RoMaterialGraphic::ResourceDescriptor() const
{
	aiva::utils::Asserts::CheckBool(mResourceDescriptor);
	return *mResourceDescriptor;
}

void aiva::layer1::RoMaterialGraphic::InitializeResourceDescriptor()
{
	mResourceDescriptor = decltype(mResourceDescriptor)::element_type::Create(mEngine);
	aiva::utils::Asserts::CheckBool(mResourceDescriptor);

	mResourceDescriptor->CacheUpdater().OnMarkAsChanged().connect(boost::bind(&RoMaterialGraphic::OnResourceDescriptorMarkedAsChanged, this));
}

void aiva::layer1::RoMaterialGraphic::TerminateResourceDescriptor()
{
	aiva::utils::Asserts::CheckBool(mResourceDescriptor);

	mResourceDescriptor->CacheUpdater().OnMarkAsChanged().disconnect(boost::bind(&RoMaterialGraphic::OnResourceDescriptorMarkedAsChanged, this));
	mResourceDescriptor = {};
}

void aiva::layer1::RoMaterialGraphic::OnResourceDescriptorMarkedAsChanged()
{
	CacheUpdater().MarkAsChanged();
}

winrt::com_ptr<ID3D12PipelineState> const& aiva::layer1::RoMaterialGraphic::InternalPipelineState() const
{
	CacheUpdater().FlushChanges();

	winrt::check_bool(mInternalPipelineState);
	return mInternalPipelineState;
}

void aiva::layer1::RoMaterialGraphic::InitializeInternalResources()
{
	CacheUpdater().FlushExecutors().connect(boost::bind(&RoMaterialGraphic::RefreshInternalResources, this));
}

void aiva::layer1::RoMaterialGraphic::TerminateInternalResources()
{
	CacheUpdater().FlushExecutors().disconnect(boost::bind(&RoMaterialGraphic::RefreshInternalResources, this));
}

void aiva::layer1::RoMaterialGraphic::RefreshInternalResources()
{
	RefreshInternalPipelineState();
}

void aiva::layer1::RoMaterialGraphic::RefreshInternalPipelineState()
{
	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	auto pipelineDesc = D3D12_GRAPHICS_PIPELINE_STATE_DESC{};

	{ // Root Signature
		auto const& rootSignature = ResourceDescriptor().InternalRootSignature();
		pipelineDesc.pRootSignature = rootSignature.get();
	}

	{ // Vertex Shader
		auto const& vertexShader = VertexShader();
		aiva::utils::Asserts::CheckBool(vertexShader);

		auto const& shaderBytecode = vertexShader->Bytecode();
		winrt::check_bool(shaderBytecode);

		pipelineDesc.VS.pShaderBytecode = shaderBytecode->GetBufferPointer();
		pipelineDesc.VS.BytecodeLength = shaderBytecode->GetBufferSize();
	}

	{ // Fragment Shader
		auto const& fragmentShader = FragmentShader();
		aiva::utils::Asserts::CheckBool(fragmentShader);

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

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::RoMaterialGraphic::PrepareBarriers(bool const active) const
{
	CacheUpdater().FlushChanges();
	return ResourceDescriptor().PrepareBarriers(active);
}
