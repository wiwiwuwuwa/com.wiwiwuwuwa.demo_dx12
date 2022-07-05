#pragma once
#include <pch.h>

#include <array>
#include <boost/core/noncopyable.hpp>
#include <winrt/base.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct Renderer final : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Main

	public:
		Renderer(aiva::layer1::Engine& engine);
		~Renderer();

	private:
		void OnEngineRender();

		aiva::layer1::Engine& mEngine;

	// ----------------------------------------------------
	// DirectX

	private:
		static constexpr int32_t SWAP_CHAIN_BUFFERS_COUNT = 2;
		static constexpr bool SWAP_CHAIN_VSYNC_ENABLED = false;

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

		static winrt::com_ptr<ID3D12DescriptorHeap> CreateDescriptorHeap(winrt::com_ptr<ID3D12Device9> const& device);

		static std::array<winrt::com_ptr<ID3D12Resource>, SWAP_CHAIN_BUFFERS_COUNT> CreateRenderTargetViews(winrt::com_ptr<ID3D12Device9> const& device, winrt::com_ptr<IDXGISwapChain4> const& swapChain, winrt::com_ptr<ID3D12DescriptorHeap> const& descriptorHeap);

		static winrt::com_ptr<ID3D12CommandAllocator> CreateCommandAllocator(winrt::com_ptr<ID3D12Device9> const& device);

		static winrt::com_ptr<ID3D12GraphicsCommandList6> CreateCommandList(winrt::com_ptr<ID3D12Device9> const& device, winrt::com_ptr<ID3D12CommandAllocator> const& commandAllocator);

		static winrt::com_ptr<ID3D12Fence1> CreateFence(winrt::com_ptr<ID3D12Device9> const& device, uint64_t const tick);

		static void WaitFrame(winrt::com_ptr<ID3D12Fence1> const& fence, uint64_t const desiredFrame);

		static void PresentFrame(winrt::com_ptr<IDXGISwapChain4> const& swapChain, bool const isTearingAllowed);

		static void ResetCommandList(winrt::com_ptr<ID3D12CommandAllocator> const& commandAllocator, winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList);

		static void PopulateCommandList(winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList);

		static void CloseCommandList(winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList);

		static void ExecuteCommandList(winrt::com_ptr<ID3D12CommandQueue> const& commandQueue, winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList);

		static void ExecuteSignalForFrame(winrt::com_ptr<ID3D12CommandQueue> const& commandQueue, winrt::com_ptr<ID3D12Fence1> const& fence, uint64_t frame);

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

		winrt::com_ptr<ID3D12DescriptorHeap> mDescriptorHeap{};

		std::array<winrt::com_ptr<ID3D12Resource>, SWAP_CHAIN_BUFFERS_COUNT> mRenderTargetViews{};

		winrt::com_ptr<ID3D12CommandAllocator> mCommandAllocator{};

		winrt::com_ptr<ID3D12GraphicsCommandList6> mCommandList{};

		winrt::com_ptr<ID3D12Fence1> mFence{};
	};
}
