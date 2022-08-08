#include <pch.h>
#include <aiva/layer1/graphic_hardware.h>

#include <aiva/layer0/app.h>
#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_texture_2d.h>
#include <aiva/layer1/grv_rtv_to_texture_2d.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/object_factory.h>

aiva::layer1::GraphicHardware::GraphicHardware(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeDirectX();
	InitializeScreenRenderTargets();
}

aiva::layer1::GraphicHardware::~GraphicHardware()
{
	TerminateScreenRenderTargets();
	TerminateDirectX();
}

void aiva::layer1::GraphicHardware::InitializeDirectX()
{
#if defined(_DEBUG)
	EnableDebugLayer();
#endif

	mFactory = CreateFactory();
	winrt::check_bool(mFactory);

	mAdapter = CreateAdapter(mFactory);
	winrt::check_bool(mAdapter);

	mDevice = CreateDevice(mAdapter);
	winrt::check_bool(mDevice);

#if defined(_DEBUG)
	mInfoQueue = CreateInfoQueue(mDevice);
	winrt::check_bool(mInfoQueue);
#endif

	mCommandQueue = CreateCommandQueue(mDevice);
	winrt::check_bool(mCommandQueue);

	mIsTearingAllowed = CheckIsTearingAllowed(mFactory);

	mSwapChain = CreateSwapChain(mFactory, mCommandQueue, mEngine.App()->Window(), mIsTearingAllowed);
	winrt::check_bool(mSwapChain);

	mCommandAllocator = CreateCommandAllocator(mDevice);
	winrt::check_bool(mCommandAllocator);

	mCommandList = CreateCommandList(mDevice, mCommandAllocator);
	winrt::check_bool(mCommandList);

	mFence = CreateFence(mDevice, mEngine.Tick());
	winrt::check_bool(mFence);
}

void aiva::layer1::GraphicHardware::TerminateDirectX()
{
	mFence = {};
	mCommandList = {};
	mCommandAllocator = {};
	mSwapChain = {};
	mIsTearingAllowed = {};
	mCommandQueue = {};
#if defined(_DEBUG)
	mInfoQueue = {};
#endif
	mDevice = {};
	mAdapter = {};
	mFactory = {};
}

#if defined(_DEBUG)
void aiva::layer1::GraphicHardware::EnableDebugLayer()
{
	winrt::com_ptr<ID3D12Debug> debug{};
	winrt::check_hresult(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)));
	debug->EnableDebugLayer();
}
#endif

winrt::com_ptr<IDXGIFactory7> aiva::layer1::GraphicHardware::CreateFactory()
{
#if defined(_DEBUG)
	const UINT flags = DXGI_CREATE_FACTORY_DEBUG;
#else
	const UINT flags = 0;
#endif

	winrt::com_ptr<IDXGIFactory2> basicFactory{};
	winrt::check_hresult(CreateDXGIFactory2(flags, IID_PPV_ARGS(&basicFactory)));

	winrt::com_ptr<IDXGIFactory7> specificFactory{};
	basicFactory.as(specificFactory);

	return specificFactory;
}

winrt::com_ptr<IDXGIAdapter4> aiva::layer1::GraphicHardware::CreateAdapter(winrt::com_ptr<IDXGIFactory7> const& factory)
{
	winrt::check_bool(factory);

	winrt::com_ptr<IDXGIAdapter> basicAdapter{};
	winrt::check_hresult(factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&basicAdapter)));

	winrt::com_ptr<IDXGIAdapter4> specificAdapter{};
	basicAdapter.as(specificAdapter);

	return specificAdapter;
}

winrt::com_ptr<ID3D12Device9> aiva::layer1::GraphicHardware::CreateDevice(winrt::com_ptr<IDXGIAdapter4> const& adapter)
{
	winrt::check_bool(adapter);

	winrt::com_ptr<ID3D12Device> basicDevice{};
	winrt::check_hresult(D3D12CreateDevice(adapter.get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&basicDevice)));

	winrt::com_ptr<ID3D12Device9> specificDevice{};
	basicDevice.as(specificDevice);

	return specificDevice;
}

#if defined(_DEBUG)
winrt::com_ptr<ID3D12InfoQueue1> aiva::layer1::GraphicHardware::CreateInfoQueue(winrt::com_ptr<ID3D12Device9> const& device)
{
	winrt::check_bool(device);

	winrt::com_ptr<ID3D12InfoQueue1> infoQueue{};
	device.as(infoQueue);

	infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
	infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

	{ // Ignore empty index buffer warnings
		auto filter = D3D12_INFO_QUEUE_FILTER{};

		std::vector<D3D12_MESSAGE_SEVERITY> severities = { D3D12_MESSAGE_SEVERITY_WARNING };
		filter.DenyList.NumSeverities = severities.size();
		filter.DenyList.pSeverityList = severities.data();

		std::vector<D3D12_MESSAGE_ID> messageIDs = { D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INDEX_BUFFER_NOT_SET };
		filter.DenyList.NumIDs = messageIDs.size();
		filter.DenyList.pIDList = messageIDs.data();

		infoQueue->PushStorageFilter(&filter);
	}

	return infoQueue;
}
#endif

winrt::com_ptr<ID3D12CommandQueue> aiva::layer1::GraphicHardware::CreateCommandQueue(winrt::com_ptr<ID3D12Device9> const& device)
{
	winrt::check_bool(device);

	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	winrt::com_ptr<ID3D12CommandQueue> commandQueue{};
	winrt::check_hresult(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue)));

	return commandQueue;
}

bool aiva::layer1::GraphicHardware::CheckIsTearingAllowed(winrt::com_ptr<IDXGIFactory7> const& factory)
{
	winrt::check_bool(factory);

	BOOL isTearingAllowed{};
	winrt::check_hresult(factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &isTearingAllowed, sizeof(isTearingAllowed)));

	return isTearingAllowed;
}

winrt::com_ptr<IDXGISwapChain4> aiva::layer1::GraphicHardware::CreateSwapChain(winrt::com_ptr<IDXGIFactory7> const& factory, winrt::com_ptr<ID3D12CommandQueue> const& commandQueue, CoreWindow const window, bool const isTearingAllowed)
{
	winrt::check_bool(factory);
	winrt::check_bool(commandQueue);
	winrt::check_bool(window);

	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.Width = 0;
	desc.Height = 0;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Stereo = FALSE;
	desc.SampleDesc = DXGI_SAMPLE_DESC{ 1, 0 };
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = SWAP_CHAIN_BUFFERS_COUNT;
	desc.Scaling = DXGI_SCALING_STRETCH;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.Flags = isTearingAllowed ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

	winrt::com_ptr<IDXGISwapChain1> basicSwapChain{};
	winrt::check_hresult(factory->CreateSwapChainForCoreWindow(commandQueue.get(), winrt::get_unknown(window), &desc, nullptr, basicSwapChain.put()));

	winrt::com_ptr<IDXGISwapChain4> specificSwapChain{};
	basicSwapChain.as(specificSwapChain);

	return specificSwapChain;
}

winrt::com_ptr<ID3D12CommandAllocator> aiva::layer1::GraphicHardware::CreateCommandAllocator(winrt::com_ptr<ID3D12Device9> const& device)
{
	winrt::check_bool(device);

	winrt::com_ptr<ID3D12CommandAllocator> commandAllocator{};
	winrt::check_hresult(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));

	return commandAllocator;
}

winrt::com_ptr<ID3D12GraphicsCommandList6> aiva::layer1::GraphicHardware::CreateCommandList(winrt::com_ptr<ID3D12Device9> const& device, winrt::com_ptr<ID3D12CommandAllocator> const& commandAllocator)
{
	winrt::check_bool(device);
	winrt::check_bool(commandAllocator);

	winrt::com_ptr<ID3D12GraphicsCommandList> basicCommandList{};
	winrt::check_hresult(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.get(), nullptr, IID_PPV_ARGS(&basicCommandList)));

	winrt::com_ptr<ID3D12GraphicsCommandList6> specificCommandList{};
	basicCommandList.as(specificCommandList);

	winrt::check_hresult(specificCommandList->Close());
	return specificCommandList;
}

winrt::com_ptr<ID3D12Fence1> aiva::layer1::GraphicHardware::CreateFence(winrt::com_ptr<ID3D12Device9> const& device, uint64_t const tick)
{
	winrt::check_bool(device);

	winrt::com_ptr<ID3D12Fence> basicFence{};
	winrt::check_hresult(device->CreateFence(tick, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&basicFence)));

	winrt::com_ptr<ID3D12Fence1> specificFence{};
	basicFence.as(specificFence);

	return specificFence;
}

winrt::com_ptr<IDXGIFactory7> const& aiva::layer1::GraphicHardware::Factory() const
{
	return mFactory;
}

winrt::com_ptr<IDXGIAdapter4> const& aiva::layer1::GraphicHardware::Adapter() const
{
	return mAdapter;
}

winrt::com_ptr<ID3D12Device9> const& aiva::layer1::GraphicHardware::Device() const
{
	return mDevice;
}

#if defined(_DEBUG)
winrt::com_ptr<ID3D12InfoQueue1> const& aiva::layer1::GraphicHardware::InfoQueue() const
{
	return mInfoQueue;
}
#endif

winrt::com_ptr<ID3D12CommandQueue> const& aiva::layer1::GraphicHardware::CommandQueue() const
{
	return mCommandQueue;
}

bool aiva::layer1::GraphicHardware::IsTearingAllowed() const
{
	return mIsTearingAllowed;
}

winrt::com_ptr<IDXGISwapChain4> const& aiva::layer1::GraphicHardware::SwapChain() const
{
	return mSwapChain;
}

winrt::com_ptr<ID3D12CommandAllocator> const& aiva::layer1::GraphicHardware::CommandAllocator() const
{
	return mCommandAllocator;
}

winrt::com_ptr<ID3D12GraphicsCommandList6> const& aiva::layer1::GraphicHardware::CommandList() const
{
	return mCommandList;
}

winrt::com_ptr<ID3D12Fence1> const& aiva::layer1::GraphicHardware::Fence() const
{
	return mFence;
}

std::shared_ptr<aiva::layer1::ResourceViewHeap>  aiva::layer1::GraphicHardware::ScreenViewHeap() const
{
	aiva::utils::Asserts::CheckBool(mScreenRenderTargets);
	return mScreenRenderTargets;
}

std::string aiva::layer1::GraphicHardware::ScreenViewKey() const
{
	auto const& swapChain = SwapChain();
	winrt::check_bool(swapChain);

	return std::to_string(swapChain->GetCurrentBackBufferIndex());
}

std::shared_ptr<aiva::layer1::GrvRtvToTexture2D> aiva::layer1::GraphicHardware::ScreenViewObj() const
{
	auto const& viewHeap = ScreenViewHeap();
	aiva::utils::Asserts::CheckBool(viewHeap);

	auto const& viewKey = ScreenViewKey();
	aiva::utils::Asserts::CheckBool(!viewKey.empty());

	auto const& viewObj = viewHeap->GetView<GrvRtvToTexture2D>(viewKey);
	aiva::utils::Asserts::CheckBool(viewObj);

	return viewObj;
}

winrt::com_ptr<ID3D12Resource> aiva::layer1::GraphicHardware::ScreenViewRes() const
{
	auto const& viewObj = ScreenViewObj();
	aiva::utils::Asserts::CheckBool(viewObj);

	auto const& viewBuf = viewObj->InternalResource();
	aiva::utils::Asserts::CheckBool(viewBuf);

	auto const& viewRes = viewBuf->InternalResource();
	winrt::check_bool(viewRes);

	return viewRes;
}

D3D12_CPU_DESCRIPTOR_HANDLE aiva::layer1::GraphicHardware::ScreenViewHandle() const
{
	auto const& viewHeap = ScreenViewHeap();
	aiva::utils::Asserts::CheckBool(viewHeap);

	auto const& viewKey = ScreenViewKey();
	aiva::utils::Asserts::CheckBool(!viewKey.empty());

	auto const& viewHandle = viewHeap->InternalDescriptorHandle(viewKey);
	aiva::utils::Asserts::CheckBool(viewHandle);

	return *viewHandle;
}

glm::vec4 aiva::layer1::GraphicHardware::ScreenViewRect() const
{
	auto const viewRes = ScreenViewRes();
	winrt::check_bool(viewRes);

	auto const viewDesc = viewRes->GetDesc();

	auto viewRect = glm::vec4{};
	viewRect.x = 0.0f;
	viewRect.y = 0.0f;
	viewRect.z = viewDesc.Width;
	viewRect.w = viewDesc.Height;

	return viewRect;
}

void aiva::layer1::GraphicHardware::InitializeScreenRenderTargets()
{
	auto const& screenRenderTargets = ResourceViewHeap::FactoryType::Create<ResourceViewHeap>(mEngine, EDescriptorHeapType::Rtv);
	aiva::utils::Asserts::CheckBool(screenRenderTargets);

	auto const& swapChain = SwapChain();
	winrt::check_bool(swapChain);

	auto swapDesc = DXGI_SWAP_CHAIN_DESC1{};
	winrt::check_hresult(swapChain->GetDesc1(&swapDesc));

	for (std::size_t i = {}; i < std::size_t{ swapDesc.BufferCount }; i++)
	{
		auto directxResource = winrt::com_ptr<ID3D12Resource>{};
		winrt::check_hresult(swapChain->GetBuffer(i, IID_PPV_ARGS(&directxResource)));

		auto aivaTexture = GrTexture2D::FactoryType::Create<GrTexture2D>(mEngine);
		aivaTexture->InternalResource(directxResource);

		auto aivaView = GrvRtvToTexture2D::FactoryType::Create<GrvRtvToTexture2D>(mEngine);
		aivaView->InternalResource(aivaTexture);

		auto const key = std::to_string(i);
		aiva::utils::Asserts::CheckBool(key.size() == 1, "Screen RT sorting will be incorrect");

		screenRenderTargets->SetView(key, aivaView);
	}

	mScreenRenderTargets = screenRenderTargets;
}

void aiva::layer1::GraphicHardware::TerminateScreenRenderTargets()
{
	mScreenRenderTargets = {};
}
