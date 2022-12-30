#pragma once
#include <pch.h>

#include <aiva2/engine/i_object.h>
#include <aiva2/engine/m_field_by_ref.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/native/engine_fwd.h>
#include <aiva2/native/uwp/graphic_hardware_fwd.h>

namespace aiva2::native
{
	struct GraphicHardware final : public engine::IObject
	{
	// ----------------------------------------------------
	// Main

	public:
		M_STRUCT_BODY(GraphicHardware);

	public:
		GraphicHardware(native::Engine& engine);

		~GraphicHardware() override;

	public:
		M_FIELD_BY_REF_3(public, native::Engine, Engine);

	// ----------------------------------------------------
	// Components

	private:
		void InitComponents();

		void ShutComponents();

	// ----------------------------------------------------

#if defined(_DEBUG)
	public:
		auto Debug() const -> ID3D12Debug6&;

	private:
		void InitDebug();

		void ShutDebug();

	private:
		winrt::com_ptr<ID3D12Debug6> mDebug{};
#endif

	// ----------------------------------------------------

	public:
		auto Factory() const -> IDXGIFactory7&;

	private:
		void InitFactory();

		void ShutFactory();

	private:
		winrt::com_ptr<IDXGIFactory7> mFactory{};

	// ----------------------------------------------------

	public:
		auto Adapter() const -> IDXGIAdapter4&;

	private:
		void InitAdapter();

		void ShutAdapter();

	private:
		winrt::com_ptr<IDXGIAdapter4> mAdapter{};

	// ----------------------------------------------------

	public:
		auto Device() const -> ID3D12Device9&;

	private:
		void InitDevice();

		void ShutDevice();

	private:
		winrt::com_ptr<ID3D12Device9> mDevice{};

	// ----------------------------------------------------

#if defined(_DEBUG)
	public:
		auto InfoQueue() const -> ID3D12InfoQueue1&;

	private:
		void InitInfoQueue();

		void ShutInfoQueue();

	private:
		winrt::com_ptr<ID3D12InfoQueue1> mInfoQueue{};
#endif

	// ----------------------------------------------------

	public:
		auto CommandQueue() const -> ID3D12CommandQueue&;

	private:
		void InitCommandQueue();

		void ShutCommandQueue();

	private:
		winrt::com_ptr<ID3D12CommandQueue> mCommandQueue{};

	// ----------------------------------------------------

	public:
		auto IsTearingAllowed() const -> bool;

	private:
		void InitIsTearingAllowed();

		void ShutIsTearingAllowed();

	private:
		std::optional<bool> mIsTearingAllowed{};

	// ----------------------------------------------------

	public:
		auto SwapChain() const -> IDXGISwapChain4&;

	private:
		void InitSwapChain();

		void ShutSwapChain();

	private:
		winrt::com_ptr<IDXGISwapChain4> mSwapChain{};

	// ----------------------------------------------------

	public:
		auto CommandAllocator() const -> ID3D12CommandAllocator&;

	private:
		void InitCommandAllocator();

		void ShutCommandAllocator();

	private:
		winrt::com_ptr<ID3D12CommandAllocator> mCommandAllocator{};

	// ----------------------------------------------------

	public:
		auto CommandList() const -> ID3D12GraphicsCommandList6&;

	private:
		void InitCommandList();

		void ShutCommandList();

	private:
		winrt::com_ptr<ID3D12GraphicsCommandList6> mCommandList{};

	// ----------------------------------------------------

	public:
		auto Fence() const -> ID3D12Fence1&;

	private:
		void InitFence();

		void ShutFence();

	private:
		winrt::com_ptr<ID3D12Fence1> mFence{};
	};
}
