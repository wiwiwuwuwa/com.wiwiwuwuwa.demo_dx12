#pragma once

#include "pch.h"

namespace aiva
{
	struct Engine;

	class Renderer final : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Renderer

	public:
		Renderer(winrt::com_ptr<aiva::Engine> const& engine);

	private:
		winrt::com_ptr<aiva::Engine> mEngine;

	// ----------------------------------------------------
	// DirectX

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

	private:
		winrt::com_ptr<IDXGIFactory7> mFactory{};

		winrt::com_ptr<IDXGIAdapter4> mAdapter{};

		winrt::com_ptr<ID3D12Device9> mDevice{};

#if defined(_DEBUG)
		winrt::com_ptr<ID3D12InfoQueue1> mInfoQueue{};
#endif

		winrt::com_ptr<ID3D12CommandQueue> mCommandQueue{};

		bool mIsTearingAllowed;

		winrt::com_ptr<IDXGISwapChain4> mSwapChain{};
	};
}
