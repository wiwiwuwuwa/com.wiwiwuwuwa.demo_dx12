#include <pch.h>
#include <aiva/layer1/ro_material_compute.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/ro_shader_compute.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/utils/object_utils.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::RoMaterialCompute::RoMaterialCompute(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
	InitializeResourceDescriptor();
	InitializeInternalResources();
}

aiva::layer1::RoMaterialCompute::~RoMaterialCompute()
{
	TerminateInternalResources();
	TerminateResourceDescriptor();
	TerminateCacheUpdater();
}

void aiva::layer1::RoMaterialCompute::DeserealizeFromBinary(std::vector<std::byte> const& binaryData)
{
	aiva::utils::Asserts::CheckBool(!binaryData.empty());

	auto root = nlohmann::json::parse(binaryData);
	{
		auto shader = root.at("shader").get<std::string>();
		Shader(mEngine.ResourceSystem().GetResource<aiva::layer1::RoShaderCompute>(shader));
	}

	CacheUpdater().MarkAsChanged();
}

aiva::layer1::RoMaterialCompute::CacheUpdaterType& aiva::layer1::RoMaterialCompute::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::RoMaterialCompute::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::RoMaterialCompute::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

std::shared_ptr<aiva::layer1::RoShaderCompute> aiva::layer1::RoMaterialCompute::Shader() const
{
	return mShader;
}

aiva::layer1::RoMaterialCompute& aiva::layer1::RoMaterialCompute::Shader(std::shared_ptr<RoShaderCompute> const& shader)
{
	mShader = shader;
	CacheUpdater().MarkAsChanged();

	return *this;
}

aiva::layer1::MaterialResourceDescriptor& aiva::layer1::RoMaterialCompute::ResourceDescriptor() const
{
	aiva::utils::Asserts::CheckBool(mResourceDescriptor);
	return *mResourceDescriptor;
}

void aiva::layer1::RoMaterialCompute::InitializeResourceDescriptor()
{
	mResourceDescriptor = aiva::utils::NewObject<decltype(mResourceDescriptor)::element_type>(mEngine);
	aiva::utils::Asserts::CheckBool(mResourceDescriptor);

	mResourceDescriptor->OnMarkCacheDataAsChanged().connect(boost::bind(&RoMaterialCompute::OnResourceDescriptorMarkedAsChanged, this));
}

void aiva::layer1::RoMaterialCompute::TerminateResourceDescriptor()
{
	aiva::utils::Asserts::CheckBool(mResourceDescriptor);

	mResourceDescriptor->OnMarkCacheDataAsChanged().disconnect(boost::bind(&RoMaterialCompute::OnResourceDescriptorMarkedAsChanged, this));
	mResourceDescriptor = {};
}

void aiva::layer1::RoMaterialCompute::OnResourceDescriptorMarkedAsChanged()
{
	CacheUpdater().MarkAsChanged();
}

winrt::com_ptr<ID3D12PipelineState> const& aiva::layer1::RoMaterialCompute::InternalPipelineState() const
{
	CacheUpdater().FlushChanges();

	winrt::check_bool(mInternalPipelineState);
	return mInternalPipelineState;
}

void aiva::layer1::RoMaterialCompute::InitializeInternalResources()
{
	CacheUpdater().FlushExecutors().connect(boost::bind(&RoMaterialCompute::RefreshInternalResources, this));
}

void aiva::layer1::RoMaterialCompute::TerminateInternalResources()
{
	CacheUpdater().FlushExecutors().disconnect(boost::bind(&RoMaterialCompute::RefreshInternalResources, this));
}

void aiva::layer1::RoMaterialCompute::RefreshInternalResources()
{
	RefreshInternalPipelineState();
}

void aiva::layer1::RoMaterialCompute::RefreshInternalPipelineState()
{
	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	auto pipelineDesc = D3D12_COMPUTE_PIPELINE_STATE_DESC{};

	{ // Root Signature
		auto const& rootSignature = ResourceDescriptor().InternalRootSignature();
		pipelineDesc.pRootSignature = rootSignature.get();
	}

	{ // Shader
		auto const& shader = Shader();
		aiva::utils::Asserts::CheckBool(shader);

		auto const& shaderBytecode = shader->Bytecode();
		winrt::check_bool(shaderBytecode);

		pipelineDesc.CS.pShaderBytecode = shaderBytecode->GetBufferPointer();
		pipelineDesc.CS.BytecodeLength = shaderBytecode->GetBufferSize();
	}

	auto pipelineState = winrt::com_ptr<ID3D12PipelineState>{};
	winrt::check_hresult(device->CreateComputePipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState)));

	winrt::check_bool(pipelineState);
	mInternalPipelineState = pipelineState;
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::RoMaterialCompute::PrepareBarriers(bool const active) const
{
	CacheUpdater().FlushChanges();
	return ResourceDescriptor().PrepareBarriers(active);
}
