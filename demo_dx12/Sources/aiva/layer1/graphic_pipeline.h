#pragma once
#include <pch.h>

#include <boost/core/noncopyable.hpp>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct GraphicPipeline final : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Main

	public:
		GraphicPipeline(aiva::layer1::Engine& engine);

		~GraphicPipeline();

	private:
		aiva::layer1::Engine& mEngine;

	// ----------------------------------------------------
	// Events

	public:
		aiva::utils::EvAction& OnCleanupCommands();
		
		aiva::utils::EvAction& OnPopulateCommands();

		aiva::utils::EvAction& OnExecuteCommands();

	private:
		aiva::utils::EvAction mOnCleanupCommands{};

		aiva::utils::EvAction mOnPopulateCommands{};

		aiva::utils::EvAction mOnExecuteCommands{};

	// ----------------------------------------------------
	// DirectX

	private:
		static constexpr bool SWAP_CHAIN_VSYNC_ENABLED = false;

	private:
		void InitializeDirectX();

		void TickDirectX();

		void TerminateDirectX();

	private:
		static void WaitFrame(winrt::com_ptr<ID3D12Fence1> const& fence, uint64_t const desiredFrame);

		static void PresentFrame(winrt::com_ptr<IDXGISwapChain4> const& swapChain, bool const isTearingAllowed);

		static void ResetCommandList(winrt::com_ptr<ID3D12CommandAllocator> const& commandAllocator, winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList);

		static void CloseCommandList(winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList);

		static void ExecuteCommandList(winrt::com_ptr<ID3D12CommandQueue> const& commandQueue, winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList);

		static void ExecuteSignalForFrame(winrt::com_ptr<ID3D12CommandQueue> const& commandQueue, winrt::com_ptr<ID3D12Fence1> const& fence, uint64_t frame);
	};
}
