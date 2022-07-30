#pragma once
#include <pch.h>

#include <array>
#include <boost/core/noncopyable.hpp>
#include <winrt/base.h>

namespace aiva::layer1
{
	struct Engine;
	struct GrvRtvToTexture2D;
	struct ResourceViewHeap;
}

namespace aiva::layer1
{
	struct GraphicHardware final : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Main

	public:
		GraphicHardware(aiva::layer1::Engine const& engine);

		~GraphicHardware();

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// DirectX Logic

	private:
		static constexpr int32_t SWAP_CHAIN_BUFFERS_COUNT = 2;

	private:
		void InitializeDirectX();

		void TerminateDirectX();

	private:
#if defined(_DEBUG)
		void EnableDebugLayer();
#endif

		static winrt::com_ptr<IDXGIFactory7> CreateFactory();

		static winrt::com_ptr<IDXGIAdapter4> CreateAdapter(winrt::com_ptr<IDXGIFactory7> const& factory);

		static winrt::com_ptr<ID3D12Device9> CreateDevice(winrt::com_ptr<IDXGIAdapter4> const& adapter);

#if defined(_DEBUG)
		static winrt::com_ptr<ID3D12InfoQueue1> CreateInfoQueue(winrt::com_ptr<ID3D12Device9> const& device);
#endif

		static winrt::com_ptr<ID3D12CommandQueue> CreateCommandQueue(winrt::com_ptr<ID3D12Device9> const& device);

		static bool CheckIsTearingAllowed(winrt::com_ptr<IDXGIFactory7> const& factory);

		static winrt::com_ptr<IDXGISwapChain4> CreateSwapChain(winrt::com_ptr<IDXGIFactory7> const& factory, winrt::com_ptr<ID3D12CommandQueue> const& commandQueue, CoreWindow const window, bool const isTearingAllowed);

		static winrt::com_ptr<ID3D12CommandAllocator> CreateCommandAllocator(winrt::com_ptr<ID3D12Device9> const& device);

		static winrt::com_ptr<ID3D12GraphicsCommandList6> CreateCommandList(winrt::com_ptr<ID3D12Device9> const& device, winrt::com_ptr<ID3D12CommandAllocator> const& commandAllocator);

		static winrt::com_ptr<ID3D12Fence1> CreateFence(winrt::com_ptr<ID3D12Device9> const& device, uint64_t const tick);

	// ----------------------------------------------------
	// DirectX Data

	public:
		winrt::com_ptr<IDXGIFactory7> const& Factory() const;

		winrt::com_ptr<IDXGIAdapter4> const& Adapter() const;

		winrt::com_ptr<ID3D12Device9> const& Device() const;

#if defined(_DEBUG)
		winrt::com_ptr<ID3D12InfoQueue1> const& InfoQueue() const;
#endif

		winrt::com_ptr<ID3D12CommandQueue> const& CommandQueue() const;

		bool IsTearingAllowed() const;

		winrt::com_ptr<IDXGISwapChain4> const& SwapChain() const;

		winrt::com_ptr<ID3D12CommandAllocator> const& CommandAllocator() const;

		winrt::com_ptr<ID3D12GraphicsCommandList6> const& CommandList() const;

		winrt::com_ptr<ID3D12Fence1> const& Fence() const;

	private:
		winrt::com_ptr<IDXGIFactory7> mFactory{};

		winrt::com_ptr<IDXGIAdapter4> mAdapter{};

		winrt::com_ptr<ID3D12Device9> mDevice{};

#if defined(_DEBUG)
		winrt::com_ptr<ID3D12InfoQueue1> mInfoQueue{};
#endif

		winrt::com_ptr<ID3D12CommandQueue> mCommandQueue{};

		bool mIsTearingAllowed{};

		winrt::com_ptr<IDXGISwapChain4> mSwapChain{};

		winrt::com_ptr<ID3D12CommandAllocator> mCommandAllocator{};

		winrt::com_ptr<ID3D12GraphicsCommandList6> mCommandList{};

		winrt::com_ptr<ID3D12Fence1> mFence{};

	// ----------------------------------------------------
	// Screen Render Targets

	public:
		std::shared_ptr<ResourceViewHeap> ScreenViewHeap() const;

		std::string ScreenViewKey() const;

	public:
		std::shared_ptr<GrvRtvToTexture2D> ScreenViewObj() const;

		winrt::com_ptr<ID3D12Resource> ScreenViewRes() const;

		D3D12_CPU_DESCRIPTOR_HANDLE ScreenViewHandle() const;

	private:
		void InitializeScreenRenderTargets();

		void TerminateScreenRenderTargets();

	private:
		std::shared_ptr<ResourceViewHeap> mScreenRenderTargets{};
	};
}