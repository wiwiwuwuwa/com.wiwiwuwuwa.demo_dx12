#include <pch.h>
#include <aiva/layer1/ro_material_compute.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/ro_shader_compute.h>
#include <aiva/utils/asserts.h>

aiva::layer1::RoMaterialCompute::RoMaterialCompute(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeLowLevelData();
}

aiva::layer1::RoMaterialCompute::~RoMaterialCompute()
{
	TerminateLowLevelData();
}

void aiva::layer1::RoMaterialCompute::Deserealize(std::vector<std::byte> const& binaryData)
{
	aiva::utils::Asserts::CheckBool(!binaryData.empty());
	BeginChanges();

	auto root = nlohmann::json::parse(binaryData);
	{
		auto shader = root.at("shader").get<std::string>();
		Shader(mEngine.ResourceSystem().GetResource<aiva::layer1::RoShaderCompute>(shader));
	}

	EndChanges();
}

aiva::layer1::RoMaterialCompute& aiva::layer1::RoMaterialCompute::BeginChanges()
{
	mChangesCounter.IncrementChanges();
	return *this;
}

aiva::layer1::RoMaterialCompute& aiva::layer1::RoMaterialCompute::EndChanges()
{
	mChangesCounter.DecrementChanges();
	return *this;
}

std::shared_ptr<aiva::layer1::RoShaderCompute> const& aiva::layer1::RoMaterialCompute::Shader() const
{
	aiva::utils::Asserts::CheckBool(mShader);
	return mShader;
}

aiva::layer1::RoMaterialCompute& aiva::layer1::RoMaterialCompute::Shader(std::shared_ptr<aiva::layer1::RoShaderCompute> const& shader)
{
	aiva::utils::Asserts::CheckBool(shader);

	BeginChanges();
	mShader = shader;
	EndChanges();

	return *this;
}

winrt::com_ptr<ID3D12RootSignature> const& aiva::layer1::RoMaterialCompute::RootSignature() const
{
	winrt::check_bool(mRootSignature);
	return mRootSignature;
}

void aiva::layer1::RoMaterialCompute::InitializeLowLevelData()
{
	mChangesCounter.OnChangesFinished().connect(boost::bind(&aiva::layer1::RoMaterialCompute::RefreshLowLevelData, this));
}

void aiva::layer1::RoMaterialCompute::TerminateLowLevelData()
{
	mChangesCounter.OnChangesFinished().disconnect(boost::bind(&aiva::layer1::RoMaterialCompute::RefreshLowLevelData, this));
}

void aiva::layer1::RoMaterialCompute::RefreshLowLevelData()
{
	RefreshRootSignature();
	RefreshComputePipelineState();
}

void aiva::layer1::RoMaterialCompute::RefreshRootSignature()
{
	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	D3D12_ROOT_SIGNATURE_DESC1 signatureDesc{};
	signatureDesc.NumParameters = 0;
	signatureDesc.pParameters = nullptr;
	signatureDesc.NumStaticSamplers = 0;
	signatureDesc.pStaticSamplers = nullptr;
	signatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	D3D12_VERSIONED_ROOT_SIGNATURE_DESC versionedDesc{};
	versionedDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
	versionedDesc.Desc_1_1 = signatureDesc;

	winrt::com_ptr<ID3DBlob> signatureBlob{};
	winrt::com_ptr<ID3DBlob> errorMessages{};

	HRESULT const serializationResult = D3D12SerializeVersionedRootSignature(&versionedDesc, signatureBlob.put(), errorMessages.put());
	if (FAILED(serializationResult))
	{
		winrt::check_bool(errorMessages);
		mEngine.LogToDebugConsole(static_cast<const char*>(errorMessages->GetBufferPointer()));
		winrt::throw_hresult(serializationResult);
	}

	winrt::com_ptr<ID3D12RootSignature> rootSignature{};
	winrt::check_hresult(device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));

	mRootSignature = rootSignature;
}

void aiva::layer1::RoMaterialCompute::RefreshComputePipelineState()
{
	auto const& shaderObject = Shader();
	aiva::utils::Asserts::CheckBool(shaderObject);

	auto const& shaderBytecode = shaderObject->Bytecode();
	winrt::check_bool(shaderBytecode);

	auto const& rootSignature = RootSignature();
	winrt::check_bool(rootSignature);

	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	D3D12_COMPUTE_PIPELINE_STATE_DESC pipelineDesc{};
	pipelineDesc.pRootSignature = rootSignature.get();
	pipelineDesc.CS = { shaderBytecode->GetBufferPointer(), shaderBytecode->GetBufferSize() };
	pipelineDesc.NodeMask = 0;
	pipelineDesc.CachedPSO = { nullptr, 0 };
	pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	winrt::com_ptr<ID3D12PipelineState> pipelineState{};
	winrt::check_hresult(device->CreateComputePipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState)));

	winrt::check_bool(pipelineState);
	mPipelineState = pipelineState;
}
