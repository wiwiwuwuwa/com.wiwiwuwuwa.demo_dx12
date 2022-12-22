#pragma once
#include <pch.h>

#include <aiva2/engine/graphic_hardware.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/native/uwp/engine.h>
#include <aiva2/native/uwp/graphic_hardware_fwd.h>

namespace aiva2::native
{
	struct GraphicHardware final : public engine::GraphicHardware
	{
	// ----------------------------------------------------
	// Main

	public:
		M_STRUCT_BODY(GraphicHardware);

	public:
		GraphicHardware(native::Engine& engine);

		~GraphicHardware() override;

	// ----------------------------------------------------
	// Engine (Virtual)

	public:
		native::Engine& Engine() const override;

	private:
		native::Engine& mEngine;

	// ----------------------------------------------------
	// IObject

	public:
		void Init() override;

		void Shut() override;

	// ----------------------------------------------------
	// Components

	private:
		void InitComponents();

		void ShutComponents();

	// ----------------------------------------------------

#if defined(_DEBUG)
	public:
		ID3D12Debug6& Debug() const;

	private:
		void InitDebug();

		void ShutDebug();

	private:
		winrt::com_ptr<ID3D12Debug6> mDebug{};
#endif

	// ----------------------------------------------------

	public:
		IDXGIFactory7& Factory() const;

	private:
		void InitFactory();

		void ShutFactory();

	private:
		winrt::com_ptr<IDXGIFactory7> mFactory{};

	// ----------------------------------------------------

	public:
		IDXGIAdapter4& Adapter() const;

	private:
		void InitAdapter();

		void ShutAdapter();

	private:
		winrt::com_ptr<IDXGIAdapter4> mAdapter{};

	// ----------------------------------------------------

	public:
		ID3D12Device9& Device() const;

	private:
		void InitDevice();

		void ShutDevice();

	private:
		winrt::com_ptr<ID3D12Device9> mDevice{};

	// ----------------------------------------------------

#if defined(_DEBUG)
	public:
		ID3D12InfoQueue1& InfoQueue() const;

	private:
		void InitInfoQueue();

		void ShutInfoQueue();

	private:
		winrt::com_ptr<ID3D12InfoQueue1> mInfoQueue{};
#endif

	// ----------------------------------------------------

	public:
		ID3D12CommandQueue& CommandQueue() const;

	private:
		void InitCommandQueue();

		void ShutCommandQueue();

	private:
		winrt::com_ptr<ID3D12CommandQueue> mCommandQueue{};

	// ----------------------------------------------------

	public:
		bool IsTearingAllowed() const;

	private:
		void InitIsTearingAllowed();

		void ShutIsTearingAllowed();

	private:
		std::optional<bool> mIsTearingAllowed{};

	// ----------------------------------------------------

	public:
		IDXGISwapChain4& SwapChain() const;

	private:
		void InitSwapChain();

		void ShutSwapChain();

	private:
		winrt::com_ptr<IDXGISwapChain4> mSwapChain{};

	// ----------------------------------------------------

	public:
		ID3D12CommandAllocator& CommandAllocator() const;

	private:
		void InitCommandAllocator();

		void ShutCommandAllocator();

	private:
		winrt::com_ptr<ID3D12CommandAllocator> mCommandAllocator{};

	// ----------------------------------------------------

	public:
		ID3D12GraphicsCommandList6& CommandList() const;

	private:
		void InitCommandList();

		void ShutCommandList();

	private:
		winrt::com_ptr<ID3D12GraphicsCommandList6> mCommandList{};

	// ----------------------------------------------------

	public:
		ID3D12Fence1& Fence() const;

	private:
		void InitFence();

		void ShutFence();

	private:
		winrt::com_ptr<ID3D12Fence1> mFence{};
	};
}
