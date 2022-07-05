#include <pch.h>
#include <layer1/renderer.h>

#include <boost/bind.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <layer0/app.h>
#include <layer1/engine.h>

aiva::layer1::Renderer::Renderer(aiva::layer1::Engine& engine) : mEngine{ engine }
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

	mDescriptorHeap = CreateDescriptorHeap(mDevice);
	winrt::check_bool(mDescriptorHeap);

	mRenderTargetViews = CreateRenderTargetViews(mDevice, mSwapChain, mDescriptorHeap);
	for (const winrt::com_ptr<ID3D12Resource>& rtv : mRenderTargetViews) winrt::check_bool(rtv);

	mCommandAllocator = CreateCommandAllocator(mDevice);
	winrt::check_bool(mCommandAllocator);

	mCommandList = CreateCommandList(mDevice, mCommandAllocator);
	winrt::check_bool(mCommandList);

	mFence = CreateFence(mDevice, mEngine.Tick());
	winrt::check_bool(mFence);

	mEngine.OnRender().connect(boost::bind(&aiva::layer1::Renderer::OnEngineRender, this));
}

aiva::layer1::Renderer::~Renderer()
{
	mEngine.OnRender().disconnect(boost::bind(&aiva::layer1::Renderer::OnEngineRender, this));
	mFence = {};
	mCommandList = {};
	mCommandAllocator = {};
	mRenderTargetViews = {};
	mDescriptorHeap = {};
	mSwapChain = {};
	mIsTearingAllowed = {};
	mCommandQueue = {};
	mInfoQueue = {};
	mDevice = {};
	mAdapter = {};
	mFactory = {};
}

void aiva::layer1::Renderer::OnEngineRender()
{
	winrt::check_bool(mFence);
	winrt::check_bool(mCommandQueue);
	winrt::check_bool(mCommandAllocator);
	winrt::check_bool(mCommandList);
	winrt::check_bool(mSwapChain);

	WaitFrame(mFence, mEngine.Tick());
	PresentFrame(mSwapChain, mIsTearingAllowed);
	ResetCommandList(mCommandAllocator, mCommandList);
	PopulateCommandList(mDevice, mCommandList, mSwapChain, mDescriptorHeap, mRenderTargetViews);
	CloseCommandList(mCommandList);
	ExecuteCommandList(mCommandQueue, mCommandList);
	ExecuteSignalForFrame(mCommandQueue, mFence, mEngine.Tick() + 1);
}

#if defined(_DEBUG)
void aiva::layer1::Renderer::EnableDebugLayer()
{
	winrt::com_ptr<ID3D12Debug> debug{};
	winrt::check_hresult(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)));
	debug->EnableDebugLayer();
}
#endif

winrt::com_ptr<IDXGIFactory7> aiva::layer1::Renderer::CreateFactory()
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

winrt::com_ptr<IDXGIAdapter4> aiva::layer1::Renderer::CreateAdapter(winrt::com_ptr<IDXGIFactory7> const& factory)
{
	winrt::check_bool(factory);

	winrt::com_ptr<IDXGIAdapter> basicAdapter{};
	winrt::check_hresult(factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&basicAdapter)));

	winrt::com_ptr<IDXGIAdapter4> specificAdapter{};
	basicAdapter.as(specificAdapter);

	return specificAdapter;
}

winrt::com_ptr<ID3D12Device9> aiva::layer1::Renderer::CreateDevice(winrt::com_ptr<IDXGIAdapter4> const& adapter)
{
	winrt::check_bool(adapter);

	winrt::com_ptr<ID3D12Device> basicDevice{};
	winrt::check_hresult(D3D12CreateDevice(adapter.get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&basicDevice)));

	winrt::com_ptr<ID3D12Device9> specificDevice{};
	basicDevice.as(specificDevice);

	return specificDevice;
}

#if defined(_DEBUG)
winrt::com_ptr<ID3D12InfoQueue1> aiva::layer1::Renderer::CreateInfoQueue(winrt::com_ptr<ID3D12Device9> const& device)
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

winrt::com_ptr<ID3D12CommandQueue> aiva::layer1::Renderer::CreateCommandQueue(winrt::com_ptr<ID3D12Device9> const& device)
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

bool aiva::layer1::Renderer::CheckIsTearingAllowed(winrt::com_ptr<IDXGIFactory7> const& factory)
{
	winrt::check_bool(factory);
	
	BOOL isTearingAllowed{};
	winrt::check_hresult(factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &isTearingAllowed, sizeof(isTearingAllowed)));

	return isTearingAllowed;
}

winrt::com_ptr<IDXGISwapChain4> aiva::layer1::Renderer::CreateSwapChain(winrt::com_ptr<IDXGIFactory7> const& factory, winrt::com_ptr<ID3D12CommandQueue> const& commandQueue, CoreWindow const window, bool const isTearingAllowed)
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

winrt::com_ptr<ID3D12DescriptorHeap> aiva::layer1::Renderer::CreateDescriptorHeap(winrt::com_ptr<ID3D12Device9> const& device)
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

std::array<winrt::com_ptr<ID3D12Resource>, aiva::layer1::Renderer::SWAP_CHAIN_BUFFERS_COUNT> aiva::layer1::Renderer::CreateRenderTargetViews(winrt::com_ptr<ID3D12Device9> const& device, winrt::com_ptr<IDXGISwapChain4> const& swapChain, winrt::com_ptr<ID3D12DescriptorHeap> const& descriptorHeap)
{
	winrt::check_bool(device);
	winrt::check_bool(swapChain);
	winrt::check_bool(descriptorHeap);

	const SIZE_T heapStart = descriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr;
	const UINT heapOffset = device->GetDescriptorHandleIncrementSize(descriptorHeap->GetDesc().Type);

	std::array<winrt::com_ptr<ID3D12Resource>, aiva::layer1::Renderer::SWAP_CHAIN_BUFFERS_COUNT> renderTargetViews{};

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

winrt::com_ptr<ID3D12CommandAllocator> aiva::layer1::Renderer::CreateCommandAllocator(winrt::com_ptr<ID3D12Device9> const& device)
{
	winrt::check_bool(device);

	winrt::com_ptr<ID3D12CommandAllocator> commandAllocator{};
	winrt::check_hresult(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));

	return commandAllocator;
}

winrt::com_ptr<ID3D12GraphicsCommandList6> aiva::layer1::Renderer::CreateCommandList(winrt::com_ptr<ID3D12Device9> const& device, winrt::com_ptr<ID3D12CommandAllocator> const& commandAllocator)
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

winrt::com_ptr<ID3D12Fence1> aiva::layer1::Renderer::CreateFence(winrt::com_ptr<ID3D12Device9> const& device, uint64_t const tick)
{
	winrt::check_bool(device);

	winrt::com_ptr<ID3D12Fence> basicFence{};
	winrt::check_hresult(device->CreateFence(tick, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&basicFence)));

	winrt::com_ptr<ID3D12Fence1> specificFence{};
	basicFence.as(specificFence);

	return specificFence;
}

void aiva::layer1::Renderer::WaitFrame(winrt::com_ptr<ID3D12Fence1> const& fence, uint64_t const desiredFrame)
{
	winrt::check_bool(fence);

	UINT64 const currentFrame = fence->GetCompletedValue();
	winrt::check_bool(currentFrame != UINT64_MAX);

	if (currentFrame >= desiredFrame)
	{
		return;
	}

	winrt::check_hresult(fence->SetEventOnCompletion(desiredFrame, nullptr));
}

void aiva::layer1::Renderer::PresentFrame(winrt::com_ptr<IDXGISwapChain4> const& swapChain, bool const isTearingAllowed)
{
	winrt::check_bool(swapChain);

	UINT const syncInterval = SWAP_CHAIN_VSYNC_ENABLED ? 1 : 0;
	UINT const presentFlags = isTearingAllowed && !SWAP_CHAIN_VSYNC_ENABLED ? DXGI_PRESENT_ALLOW_TEARING : 0;

	winrt::check_hresult(swapChain->Present(syncInterval, presentFlags));
}

void aiva::layer1::Renderer::ResetCommandList(winrt::com_ptr<ID3D12CommandAllocator> const& commandAllocator, winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList)
{
	winrt::check_bool(commandAllocator);
	winrt::check_bool(commandList);

	winrt::check_hresult(commandAllocator->Reset());
	winrt::check_hresult(commandList->Reset(commandAllocator.get(), nullptr));
}

void aiva::layer1::Renderer::PopulateCommandList(winrt::com_ptr<ID3D12Device9> const& device, winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList, winrt::com_ptr<IDXGISwapChain4> const& swapChain, winrt::com_ptr<ID3D12DescriptorHeap> const& renderTargetHeap, std::array<winrt::com_ptr<ID3D12Resource>, SWAP_CHAIN_BUFFERS_COUNT> const& renderTargetResources)
{
	winrt::check_bool(device);
	winrt::check_bool(commandList);
	winrt::check_bool(swapChain);
	winrt::check_bool(renderTargetHeap);
	for (winrt::com_ptr<ID3D12Resource> const& renderTargetResource : renderTargetResources) winrt::check_bool(renderTargetResource);

	UINT const currentBackBufferIndex = swapChain->GetCurrentBackBufferIndex();
	winrt::com_ptr<ID3D12Resource> const currentBackBufferResource = renderTargetResources.at(currentBackBufferIndex);

	{
		D3D12_RESOURCE_BARRIER openBarrier{};
		openBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		openBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		openBarrier.Transition.pResource = currentBackBufferResource.get();
		openBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		openBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		openBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		commandList->ResourceBarrier(1, &openBarrier);
	}

	{
		SIZE_T const currentBackBufferHeapStart = renderTargetHeap->GetCPUDescriptorHandleForHeapStart().ptr;
		UINT const currentBackBufferHeapDelta = device->GetDescriptorHandleIncrementSize(renderTargetHeap->GetDesc().Type);

		D3D12_CPU_DESCRIPTOR_HANDLE currentBackBufferHandle{};
		currentBackBufferHandle.ptr = currentBackBufferHeapStart + static_cast<SIZE_T>(currentBackBufferIndex * currentBackBufferHeapDelta);

		commandList->ClearRenderTargetView(currentBackBufferHandle, glm::value_ptr(BACK_BUFFER_CLEAR_COLOR), 0, nullptr);
	}

	{
		D3D12_RESOURCE_BARRIER closeBarrier{};
		closeBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		closeBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		closeBarrier.Transition.pResource = currentBackBufferResource.get();
		closeBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		closeBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		closeBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		commandList->ResourceBarrier(1, &closeBarrier);
	}
}

void aiva::layer1::Renderer::CloseCommandList(winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList)
{
	winrt::check_bool(commandList);

	commandList->Close();
}

void aiva::layer1::Renderer::ExecuteCommandList(winrt::com_ptr<ID3D12CommandQueue> const& commandQueue, winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList)
{
	winrt::check_bool(commandQueue);
	winrt::check_bool(commandList);

	std::array<ID3D12CommandList*, 1> const commandsList = { commandList.get() };
	commandQueue->ExecuteCommandLists(1, commandsList.data());
}

void aiva::layer1::Renderer::ExecuteSignalForFrame(winrt::com_ptr<ID3D12CommandQueue> const& commandQueue, winrt::com_ptr<ID3D12Fence1> const& fence, uint64_t frame)
{
	winrt::check_bool(commandQueue);
	winrt::check_bool(fence);
	
	winrt::check_hresult(commandQueue->Signal(fence.get(), frame));
}
