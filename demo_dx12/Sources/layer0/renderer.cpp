#include <pch.h>

/*
#include <core/renderer.h>
#include <core/engine.h>

aiva::Renderer::Renderer(winrt::com_ptr<aiva::Engine> const& engine)
{
	mEngine = engine;
	winrt::check_bool(mEngine);

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

	mSwapChain = CreateSwapChain(mFactory, mCommandQueue, mEngine->GetWindow(), mIsTearingAllowed);
	winrt::check_bool(mSwapChain);

	mDescriptorHeap = CreateDescriptorHeap(mDevice);
	winrt::check_bool(mDescriptorHeap);

	mRenderTargetViews = CreateRenderTargetViews(mDevice, mSwapChain, mDescriptorHeap);
	for (const winrt::com_ptr<ID3D12Resource>& rtv : mRenderTargetViews) winrt::check_bool(rtv);

	mCommandAllocator = CreateCommandAllocator(mDevice);
	winrt::check_bool(mCommandAllocator);

	mCommandList = CreateCommandList(mDevice, mCommandAllocator);
	winrt::check_bool(mCommandList);

	mFence = CreateFence(mDevice);
	winrt::check_bool(mFence);
}

#if defined(_DEBUG)
void aiva::Renderer::EnableDebugLayer()
{
	winrt::com_ptr<ID3D12Debug> debug{};
	winrt::check_hresult(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)));
	debug->EnableDebugLayer();
}
#endif

winrt::com_ptr<IDXGIFactory7> aiva::Renderer::CreateFactory()
{
#if defined(_DEBUG)
	const UINT flags = DXGI_CREATE_FACTORY_DEBUG;
#else
	cosnt UINT flags = 0;
#endif

	winrt::com_ptr<IDXGIFactory2> basicFactory{};
	winrt::check_hresult(CreateDXGIFactory2(flags, IID_PPV_ARGS(&basicFactory)));

	winrt::com_ptr<IDXGIFactory7> specificFactory{};
	basicFactory.as(specificFactory);

	return specificFactory;
}

winrt::com_ptr<IDXGIAdapter4> aiva::Renderer::CreateAdapter(winrt::com_ptr<IDXGIFactory7> const& factory)
{
	winrt::check_bool(factory);

	winrt::com_ptr<IDXGIAdapter> basicAdapter{};
	winrt::check_hresult(factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&basicAdapter)));

	winrt::com_ptr<IDXGIAdapter4> specificAdapter{};
	basicAdapter.as(specificAdapter);

	return specificAdapter;
}

winrt::com_ptr<ID3D12Device9> aiva::Renderer::CreateDevice(winrt::com_ptr<IDXGIAdapter4> const& adapter)
{
	winrt::check_bool(adapter);

	winrt::com_ptr<ID3D12Device> basicDevice{};
	winrt::check_hresult(D3D12CreateDevice(adapter.get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&basicDevice)));

	winrt::com_ptr<ID3D12Device9> specificDevice{};
	basicDevice.as(specificDevice);

	return specificDevice;
}

#if defined(_DEBUG)
winrt::com_ptr<ID3D12InfoQueue1> aiva::Renderer::CreateInfoQueue(winrt::com_ptr<ID3D12Device9> const& device)
{
	winrt::check_bool(device);

	winrt::com_ptr<ID3D12InfoQueue1> infoQueue{};
	device.as(infoQueue);

	infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
	infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

	return infoQueue;
}
#endif

winrt::com_ptr<ID3D12CommandQueue> aiva::Renderer::CreateCommandQueue(winrt::com_ptr<ID3D12Device9> const& device)
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

bool aiva::Renderer::CheckIsTearingAllowed(winrt::com_ptr<IDXGIFactory7> const& factory)
{
	winrt::check_bool(factory);
	
	BOOL isTearingAllowed{};
	winrt::check_hresult(factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &isTearingAllowed, sizeof(isTearingAllowed)));

	return isTearingAllowed;
}

winrt::com_ptr<IDXGISwapChain4> aiva::Renderer::CreateSwapChain(winrt::com_ptr<IDXGIFactory7> const& factory, winrt::com_ptr<ID3D12CommandQueue> const& commandQueue, CoreWindow const window, bool const isTearingAllowed)
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

winrt::com_ptr<ID3D12DescriptorHeap> aiva::Renderer::CreateDescriptorHeap(winrt::com_ptr<ID3D12Device9> const& device)
{
	winrt::check_bool(device);

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.NumDescriptors = static_cast<UINT>(SWAP_CHAIN_BUFFERS_COUNT);
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 0;

	winrt::com_ptr<ID3D12DescriptorHeap> descriptorHeap{};
	winrt::check_hresult(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

	return descriptorHeap;
}

std::array<winrt::com_ptr<ID3D12Resource>, aiva::Renderer::SWAP_CHAIN_BUFFERS_COUNT> aiva::Renderer::CreateRenderTargetViews(winrt::com_ptr<ID3D12Device9> const& device, winrt::com_ptr<IDXGISwapChain4> const& swapChain, winrt::com_ptr<ID3D12DescriptorHeap> const& descriptorHeap)
{
	winrt::check_bool(device);
	winrt::check_bool(swapChain);
	winrt::check_bool(descriptorHeap);

	const SIZE_T heapStart = descriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr;
	const UINT heapOffset = device->GetDescriptorHandleIncrementSize(descriptorHeap->GetDesc().Type);

	std::array<winrt::com_ptr<ID3D12Resource>, aiva::Renderer::SWAP_CHAIN_BUFFERS_COUNT> renderTargetViews{};

	for (UINT i = 0; i < renderTargetViews.size(); i++)
	{
		winrt::com_ptr<ID3D12Resource> backBuffer{};
		winrt::check_hresult(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));

		D3D12_CPU_DESCRIPTOR_HANDLE handle{};
		handle.ptr = heapStart + static_cast<SIZE_T>(heapOffset * i);

		device->CreateRenderTargetView(backBuffer.get(), nullptr, handle);
		winrt::check_bool(backBuffer);

		renderTargetViews[i] = backBuffer;
	}

	return renderTargetViews;
}

winrt::com_ptr<ID3D12CommandAllocator> aiva::Renderer::CreateCommandAllocator(winrt::com_ptr<ID3D12Device9> const& device)
{
	winrt::check_bool(device);

	winrt::com_ptr<ID3D12CommandAllocator> commandAllocator{};
	winrt::check_hresult(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));

	return commandAllocator;
}

winrt::com_ptr<ID3D12GraphicsCommandList6> aiva::Renderer::CreateCommandList(winrt::com_ptr<ID3D12Device9> const& device, winrt::com_ptr<ID3D12CommandAllocator> const& commandAllocator)
{
	winrt::check_bool(device);
	winrt::check_bool(commandAllocator);

	winrt::com_ptr<ID3D12GraphicsCommandList> basicCommandList{};
	winrt::check_hresult(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.get(), nullptr, IID_PPV_ARGS(&basicCommandList)));

	winrt::com_ptr<ID3D12GraphicsCommandList6> specificCommandList{};
	basicCommandList.as(specificCommandList);

	return specificCommandList;
}

winrt::com_ptr<ID3D12Fence1> aiva::Renderer::CreateFence(winrt::com_ptr<ID3D12Device9> const& device)
{
	winrt::check_bool(device);

	winrt::com_ptr<ID3D12Fence> basicFence{};
	winrt::check_hresult(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&basicFence)));

	winrt::com_ptr<ID3D12Fence1> specificFence{};
	basicFence.as(specificFence);

	return specificFence;
}
*/
