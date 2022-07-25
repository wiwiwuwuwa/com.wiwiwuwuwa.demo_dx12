#include <pch.h>
#include <aiva/layer1/ro_material_graphic.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/ro_shader_fragment.h>
#include <aiva/layer1/ro_shader_vertex.h>
#include <aiva/layer1/shader_resource_descriptor.h>

aiva::layer1::RoMaterialGraphic::RoMaterialGraphic(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
	InitializeResourceDescriptor();
	InitializeInternalResources();
}

aiva::layer1::RoMaterialGraphic::~RoMaterialGraphic()
{
	TerminateInternalResources();
	TerminateResourceDescriptor();
	TerminateCacheUpdater();
}

void aiva::layer1::RoMaterialGraphic::DeserealizeFromBinary(std::vector<std::byte> const& binaryData)
{
	aiva::utils::Asserts::CheckBool(!binaryData.empty());

	auto root = nlohmann::json::parse(binaryData);
	{
		auto vertexShader = root.at("vertex_shader").get<std::string>();
		VertexShader(mEngine.ResourceSystem().GetResource<aiva::layer1::RoShaderVertex>(vertexShader));

		auto fragmentShader = root.at("fragment_shader").get<std::string>();
		FragmentShader(mEngine.ResourceSystem().GetResource<aiva::layer1::RoShaderFragment>(fragmentShader));
	}

	CacheUpdater().MarkAsChanged(EDirtyFlags::All);
}

aiva::utils::TCacheRefresh<aiva::layer1::RoMaterialGraphic::EDirtyFlags>& aiva::layer1::RoMaterialGraphic::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::RoMaterialGraphic::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<decltype(mCacheUpdater)::element_type>(EDirtyFlags::All);
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
	CacheUpdater().MarkAsChanged(EDirtyFlags::All);

	return *this;
}

std::shared_ptr<aiva::layer1::RoShaderFragment> aiva::layer1::RoMaterialGraphic::FragmentShader() const
{
	return mFragmentShader;
}

aiva::layer1::RoMaterialGraphic& aiva::layer1::RoMaterialGraphic::FragmentShader(std::shared_ptr<RoShaderFragment> const& fragmentShader)
{
	mFragmentShader = fragmentShader;
	CacheUpdater().MarkAsChanged(EDirtyFlags::All);

	return *this;
}

aiva::layer1::ShaderResourceDescriptor& aiva::layer1::RoMaterialGraphic::ResourceDescriptor() const
{
	aiva::utils::Asserts::CheckBool(mResourceDescriptor);
	return *mResourceDescriptor;
}

void aiva::layer1::RoMaterialGraphic::InitializeResourceDescriptor()
{
	mResourceDescriptor = decltype(mResourceDescriptor)::element_type::Create(mEngine);
	aiva::utils::Asserts::CheckBool(mResourceDescriptor);

	mResourceDescriptor->OnInternalResourceUpdated().connect(boost::bind(&RoMaterialGraphic::OnResourceDescriptorUpdated, this));
}

void aiva::layer1::RoMaterialGraphic::TerminateResourceDescriptor()
{
	aiva::utils::Asserts::CheckBool(mResourceDescriptor);

	mResourceDescriptor->OnInternalResourceUpdated().disconnect(boost::bind(&RoMaterialGraphic::OnResourceDescriptorUpdated, this));
	mResourceDescriptor = {};
}

void aiva::layer1::RoMaterialGraphic::OnResourceDescriptorUpdated()
{
	CacheUpdater().MarkAsChanged(EDirtyFlags::All);
}

winrt::com_ptr<ID3D12PipelineState> const& aiva::layer1::RoMaterialGraphic::InternalPipelineState() const
{
	CacheUpdater().FlushChanges();

	winrt::check_bool(mInternalPipelineState);
	return mInternalPipelineState;
}

void aiva::layer1::RoMaterialGraphic::InitializeInternalResources()
{
	CacheUpdater().OnFlushRequested().connect(boost::bind(&RoMaterialGraphic::RefreshInternalResources, this));
}

void aiva::layer1::RoMaterialGraphic::TerminateInternalResources()
{
	CacheUpdater().OnFlushRequested().disconnect(boost::bind(&RoMaterialGraphic::RefreshInternalResources, this));
}

void aiva::layer1::RoMaterialGraphic::RefreshInternalResources()
{
	RefreshInternalPipelineState();
	OnInternalResourcesUpdated()();
}

void aiva::layer1::RoMaterialGraphic::RefreshInternalPipelineState()
{
	//auto const& device = mEngine.GraphicHardware().Device();
	//winrt::check_bool(device);

	//auto const& shader = Shader();
	//aiva::utils::Asserts::CheckBool(shader);

	//auto const& shaderBytecode = shader->Bytecode();
	//winrt::check_bool(shaderBytecode);

	//auto const& resourceDescriptor = ResourceDescriptor();

	//auto const& rootSignature = resourceDescriptor.InternalRootSignature();
	//winrt::check_bool(rootSignature);

	//auto pipelineDesc = D3D12_COMPUTE_PIPELINE_STATE_DESC{};
	//pipelineDesc.pRootSignature = rootSignature.get();
	//pipelineDesc.CS.pShaderBytecode = shaderBytecode->GetBufferPointer();
	//pipelineDesc.CS.BytecodeLength = shaderBytecode->GetBufferSize();
	//pipelineDesc.NodeMask = 0;
	//pipelineDesc.CachedPSO.pCachedBlob = nullptr;
	//pipelineDesc.CachedPSO.CachedBlobSizeInBytes = 0;
	//pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	//auto pipelineState = winrt::com_ptr<ID3D12PipelineState>{};
	//winrt::check_hresult(device->CreateComputePipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState)));

	//winrt::check_bool(pipelineState);
	//mInternalPipelineState = pipelineState;
}

aiva::utils::EvAction& aiva::layer1::RoMaterialGraphic::OnInternalResourcesUpdated()
{
	return mOnInternalResourcesUpdated;
}
