#include <pch.h>
#include <aiva/layer1/ro_material_compute.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/ro_shader_compute.h>
#include <aiva/layer1/shader_resource_descriptor.h>

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

	CacheUpdater().MarkAsChanged(EDirtyFlags::All);
}

aiva::utils::TCacheRefresh<aiva::layer1::RoMaterialCompute::EDirtyFlags>& aiva::layer1::RoMaterialCompute::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::RoMaterialCompute::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<decltype(mCacheUpdater)::element_type>(EDirtyFlags::All);
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
	CacheUpdater().MarkAsChanged(EDirtyFlags::All);

	return *this;
}

aiva::layer1::ShaderResourceDescriptor& aiva::layer1::RoMaterialCompute::ResourceDescriptor() const
{
	aiva::utils::Asserts::CheckBool(mResourceDescriptor);
	return *mResourceDescriptor;
}

void aiva::layer1::RoMaterialCompute::InitializeResourceDescriptor()
{
	mResourceDescriptor = decltype(mResourceDescriptor)::element_type::Create(mEngine);
	aiva::utils::Asserts::CheckBool(mResourceDescriptor);

	mResourceDescriptor->OnInternalResourceUpdated().connect(boost::bind(&RoMaterialCompute::OnResourceDescriptorUpdated, this));
}

void aiva::layer1::RoMaterialCompute::TerminateResourceDescriptor()
{
	aiva::utils::Asserts::CheckBool(mResourceDescriptor);

	mResourceDescriptor->OnInternalResourceUpdated().disconnect(boost::bind(&RoMaterialCompute::OnResourceDescriptorUpdated, this));
	mResourceDescriptor = {};
}

void aiva::layer1::RoMaterialCompute::OnResourceDescriptorUpdated()
{
	CacheUpdater().MarkAsChanged(EDirtyFlags::All);
}

winrt::com_ptr<ID3D12PipelineState> const& aiva::layer1::RoMaterialCompute::InternalPipelineState() const
{
	CacheUpdater().FlushChanges();

	winrt::check_bool(mInternalPipelineState);
	return mInternalPipelineState;
}

void aiva::layer1::RoMaterialCompute::InitializeInternalResources()
{
	CacheUpdater().OnFlushRequested().connect(boost::bind(&RoMaterialCompute::RefreshInternalResources, this));
}

void aiva::layer1::RoMaterialCompute::TerminateInternalResources()
{
	CacheUpdater().OnFlushRequested().disconnect(boost::bind(&RoMaterialCompute::RefreshInternalResources, this));
}

void aiva::layer1::RoMaterialCompute::RefreshInternalResources()
{
	RefreshInternalPipelineState();
	OnInternalResourcesUpdated()();
}

void aiva::layer1::RoMaterialCompute::RefreshInternalPipelineState()
{
	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	auto const& shader = Shader();
	aiva::utils::Asserts::CheckBool(shader);

	auto const& shaderBytecode = shader->Bytecode();
	winrt::check_bool(shaderBytecode);

	auto const& resourceDescriptor = ResourceDescriptor();

	auto const& rootSignature = resourceDescriptor.InternalRootSignature();
	winrt::check_bool(rootSignature);

	auto pipelineDesc = D3D12_COMPUTE_PIPELINE_STATE_DESC{};
	pipelineDesc.pRootSignature = rootSignature.get();
	pipelineDesc.CS.pShaderBytecode = shaderBytecode->GetBufferPointer();
	pipelineDesc.CS.BytecodeLength = shaderBytecode->GetBufferSize();
	pipelineDesc.NodeMask = 0;
	pipelineDesc.CachedPSO.pCachedBlob = nullptr;
	pipelineDesc.CachedPSO.CachedBlobSizeInBytes = 0;
	pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	auto pipelineState = winrt::com_ptr<ID3D12PipelineState>{};
	winrt::check_hresult(device->CreateComputePipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState)));

	winrt::check_bool(pipelineState);
	mInternalPipelineState = pipelineState;
}

aiva::utils::EvAction& aiva::layer1::RoMaterialCompute::OnInternalResourcesUpdated()
{
	return mOnInternalResourcesUpdated;
}
