#include <pch.h>
#include <aiva2/native/uwp/graphic_hardware.h>

#include <aiva2/engine/asserts.h>

namespace aiva2::native
{
	using namespace engine;

	GraphicHardware::GraphicHardware(native::Engine& engine) : mEngine{ engine }
	{
		
	}

	GraphicHardware::~GraphicHardware()
	{
		
	}

	native::Engine& GraphicHardware::Engine() const
	{
		return mEngine;
	}

	void GraphicHardware::Init()
	{
		engine::GraphicHardware::Init();
		InitComponents();
	}

	void GraphicHardware::Shut()
	{
		ShutComponents();
		engine::GraphicHardware::Shut();
	}

	void GraphicHardware::InitComponents()
	{
#if defined(_DEBUG)
		InitDebug();
#endif
		InitFactory();
		InitAdapter();
		InitDevice();
#if defined(_DEBUG)
		InitInfoQueue();
#endif
		InitCommandQueue();
		InitIsTearingAllowed();
		InitSwapChain();
		InitCommandAllocator();
		InitCommandList();
		InitFence();
	}

	void GraphicHardware::ShutComponents()
	{
		ShutFence();
		ShutCommandList();
		ShutCommandAllocator();
		ShutSwapChain();
		ShutIsTearingAllowed();
		ShutCommandQueue();
#if defined(_DEBUG)
		ShutInfoQueue();
#endif
		ShutDevice();
		ShutAdapter();
		ShutFactory();
#if defined(_DEBUG)
		ShutDebug();
#endif
	}

#if defined(_DEBUG)
	ID3D12Debug6& GraphicHardware::Debug() const
	{
		Asserts::IsTrue(mDebug, "Debug is not valid");
		return *mDebug;
	}

	void GraphicHardware::InitDebug()
	{
		auto basicDebug = winrt::com_ptr<ID3D12Debug>{};
		winrt::check_hresult(D3D12GetDebugInterface(IID_PPV_ARGS(&basicDebug)));
		Asserts::IsTrue(basicDebug, "Basic debug is not valid");

		auto specificDebug = winrt::com_ptr<ID3D12Debug6>{};
		basicDebug.as(specificDebug);
		Asserts::IsTrue(specificDebug, "Specific debug is not valid");

		specificDebug->EnableDebugLayer();

		mDebug = specificDebug;
	}

	void GraphicHardware::ShutDebug()
	{
		Asserts::IsTrue(mDebug, "Debug is not valid");
		mDebug = {};
	}
#endif

	IDXGIFactory7& GraphicHardware::Factory() const
	{
		Asserts::IsTrue(mFactory, "Factory is not valid");
		return *mFactory;
	}

	void GraphicHardware::InitFactory()
	{
#if defined(_DEBUG)
		auto const flags = UINT{ DXGI_CREATE_FACTORY_DEBUG };
#else
		auto const flags = UINT{};
#endif

		auto basicFactory = winrt::com_ptr<IDXGIFactory2>{};
		winrt::check_hresult(CreateDXGIFactory2(flags, IID_PPV_ARGS(&basicFactory)));
		Asserts::IsTrue(basicFactory, "Basic factory is not valid");

		auto specificFactory = winrt::com_ptr<IDXGIFactory7>{};
		basicFactory.as(specificFactory);
		Asserts::IsTrue(specificFactory, "Specific factory is not valid");

		mFactory = specificFactory;
	}

	void GraphicHardware::ShutFactory()
	{
		Asserts::IsTrue(mFactory, "Factory is not valid");
		mFactory = {};
	}

	IDXGIAdapter4& GraphicHardware::Adapter() const
	{
		Asserts::IsTrue(mAdapter, "Adapter is not valid");
		return *mAdapter;
	}

	void GraphicHardware::InitAdapter()
	{
		auto basicAdapter = winrt::com_ptr<IDXGIAdapter>{};
		winrt::check_hresult(Factory().EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&basicAdapter)));
		Asserts::IsTrue(basicAdapter, "Basic adapter is not valid");

		auto specificAdapter = winrt::com_ptr<IDXGIAdapter4>{};
		basicAdapter.as(specificAdapter);
		Asserts::IsTrue(specificAdapter, "Specific adapter is not valid");

		mAdapter = specificAdapter;
	}

	void GraphicHardware::ShutAdapter()
	{
		Asserts::IsTrue(mAdapter, "Adapter is not valid");
		mAdapter = {};
	}

	ID3D12Device9& GraphicHardware::Device() const
	{
		Asserts::IsTrue(mDevice, "Device is not valid");
		return *mDevice;
	}

	void GraphicHardware::InitDevice()
	{
		auto basicDevice = winrt::com_ptr<ID3D12Device>{};
		winrt::check_hresult(D3D12CreateDevice(&Adapter(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&basicDevice)));
		Asserts::IsTrue(basicDevice, "Basic device is not valid");

		auto specificDevice = winrt::com_ptr<ID3D12Device9>{};
		basicDevice.as(specificDevice);
		Asserts::IsTrue(specificDevice, "Specific device is not valid");

		mDevice = specificDevice;
	}

	void GraphicHardware::ShutDevice()
	{
		Asserts::IsTrue(mDevice, "Device is not valid");
		mDevice = {};
	}

#if defined(_DEBUG)
	ID3D12InfoQueue1& GraphicHardware::InfoQueue() const
	{
		Asserts::IsTrue(mInfoQueue, "Info queue is not valid");
		return *mInfoQueue;
	}

	void GraphicHardware::InitInfoQueue()
	{
		auto& device = mDevice;
		Asserts::IsTrue(device, "Device is not valid");

		auto infoQueue = winrt::com_ptr<ID3D12InfoQueue1>{};
		device.as(infoQueue);
		Asserts::IsTrue(infoQueue, "Info queue is not valid");

		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		{ // Ignore empty index buffer warnings
			auto filter = D3D12_INFO_QUEUE_FILTER{};

			auto severities = std::vector<D3D12_MESSAGE_SEVERITY>{ D3D12_MESSAGE_SEVERITY_WARNING };
			filter.DenyList.NumSeverities = static_cast<UINT>(std::size(severities));
			filter.DenyList.pSeverityList = std::data(severities);

			auto messageIDs = std::vector<D3D12_MESSAGE_ID>{ D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INDEX_BUFFER_NOT_SET };
			filter.DenyList.NumIDs = static_cast<UINT>(std::size(messageIDs));
			filter.DenyList.pIDList = std::data(messageIDs);

			infoQueue->PushStorageFilter(&filter);
		}

		mInfoQueue = infoQueue;
	}

	void GraphicHardware::ShutInfoQueue()
	{
		Asserts::IsTrue(mInfoQueue, "Info queue is not valid");
		mInfoQueue = {};
	}
#endif

	ID3D12CommandQueue& GraphicHardware::CommandQueue() const
	{
		Asserts::IsTrue(mCommandQueue, "Command queue is not valid");
		return *mCommandQueue;
	}

	void GraphicHardware::InitCommandQueue()
	{
		auto desc = D3D12_COMMAND_QUEUE_DESC{};
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = {};

		auto commandQueue = winrt::com_ptr<ID3D12CommandQueue>{};
		winrt::check_hresult(Device().CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue)));
		Asserts::IsTrue(commandQueue, "Command queue is not valid");

		mCommandQueue = commandQueue;
	}

	void GraphicHardware::ShutCommandQueue()
	{
		Asserts::IsTrue(mCommandQueue, "Command queue is not valid");
		mCommandQueue = {};
	}

	bool GraphicHardware::IsTearingAllowed() const
	{
		Asserts::IsTrue(mIsTearingAllowed, "Is tearing allowed is not valid");
		return *mIsTearingAllowed;
	}

	void GraphicHardware::InitIsTearingAllowed()
	{
		auto isTearingAllowed = BOOL{};
		winrt::check_hresult(Factory().CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &isTearingAllowed, sizeof(isTearingAllowed)));

		mIsTearingAllowed = isTearingAllowed;
	}

	void GraphicHardware::ShutIsTearingAllowed()
	{
		Asserts::IsTrue(mIsTearingAllowed, "Is tearing allowed is not valid");
		mIsTearingAllowed = {};
	}

	IDXGISwapChain4& GraphicHardware::SwapChain() const
	{
		Asserts::IsTrue(mSwapChain, "Swap chain is not valid");
		return *mSwapChain;
	}

	void GraphicHardware::InitSwapChain()
	{
		auto desc = DXGI_SWAP_CHAIN_DESC1{};
		desc.Width = {};
		desc.Height = {};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Stereo = FALSE;
		desc.SampleDesc = DXGI_SAMPLE_DESC{ 1, 0 };
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 2;
		desc.Scaling = DXGI_SCALING_STRETCH;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		desc.Flags = IsTearingAllowed() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : NULL;

		auto basicSwapChain = winrt::com_ptr<IDXGISwapChain1>{};
		winrt::check_hresult(Factory().CreateSwapChainForCoreWindow(&CommandQueue(), winrt::get_unknown(Engine().WindowSystem().Window().Window()), &desc, nullptr, basicSwapChain.put()));
		Asserts::IsTrue(basicSwapChain, "Basic swap chain is not valid");

		auto specificSwapChain = winrt::com_ptr<IDXGISwapChain4>{};
		basicSwapChain.as(specificSwapChain);
		Asserts::IsTrue(specificSwapChain, "Specific swap chain is not valid");

		mSwapChain = specificSwapChain;
	}

	void GraphicHardware::ShutSwapChain()
	{
		Asserts::IsTrue(mSwapChain, "Swap chain is not valid");
		mSwapChain = {};
	}

	ID3D12CommandAllocator& GraphicHardware::CommandAllocator() const
	{
		Asserts::IsTrue(mCommandAllocator, "Command allocator is not valid");
		return *mCommandAllocator;
	}

	void GraphicHardware::InitCommandAllocator()
	{
		auto commandAllocator = winrt::com_ptr<ID3D12CommandAllocator>{};
		winrt::check_hresult(Device().CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
		Asserts::IsTrue(commandAllocator, "Command allocator is not valid");

		mCommandAllocator = {};
	}

	void GraphicHardware::ShutCommandAllocator()
	{
		Asserts::IsTrue(mCommandAllocator, "Command allocator is not valid");
		mCommandAllocator = {};
	}

	ID3D12GraphicsCommandList6& GraphicHardware::CommandList() const
	{
		Asserts::IsTrue(mCommandList, "Command list is not valid");
		return *mCommandList;
	}

	void GraphicHardware::InitCommandList()
	{
		auto basicCommandList = winrt::com_ptr<ID3D12GraphicsCommandList>{};
		winrt::check_hresult(Device().CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, &CommandAllocator(), nullptr, IID_PPV_ARGS(&basicCommandList)));
		Asserts::IsTrue(basicCommandList, "Basic command list is not valid");

		auto specificCommandList = winrt::com_ptr<ID3D12GraphicsCommandList6>{};
		basicCommandList.as(specificCommandList);
		Asserts::IsTrue(specificCommandList, "Specific command list is not valid");

		winrt::check_hresult(specificCommandList->Close());
		
		mCommandList = specificCommandList;
	}

	void GraphicHardware::ShutCommandList()
	{
		Asserts::IsTrue(mCommandList, "Command list is not valid");
		mCommandList = {};
	}

	ID3D12Fence1& GraphicHardware::Fence() const
	{
		Asserts::IsTrue(mFence, "Fence is not valid");
		return *mFence;
	}

	void GraphicHardware::InitFence()
	{
		auto basicFence = winrt::com_ptr<ID3D12Fence>{};
		winrt::check_hresult(Device().CreateFence({}, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&basicFence)));
		Asserts::IsTrue(basicFence, "Basic fence is not valid");

		auto specificFence = winrt::com_ptr<ID3D12Fence1>{};
		basicFence.as(specificFence);
		Asserts::IsTrue(specificFence, "Specific fence is not valid");

		mFence = specificFence;
	}

	void GraphicHardware::ShutFence()
	{
		Asserts::IsTrue(mFence, "Fence is not valid");
		mFence = {};
	}
}
