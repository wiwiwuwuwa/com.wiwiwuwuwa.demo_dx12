#include <pch.h>
#include <layer1/graphic_pipeline.h>

#include <boost/bind.hpp>
#include <layer0/app.h>
#include <layer1/engine.h>
#include <layer1/graphic_hardware.h>

aiva::layer1::GraphicPipeline::GraphicPipeline(aiva::layer1::Engine& engine) : mEngine{ engine }
{
	InitializeDirectX();
}

aiva::layer1::GraphicPipeline::~GraphicPipeline()
{
	TerminateDirectX();
}

aiva::utils::EvAction& aiva::layer1::GraphicPipeline::OnPopulateCommands()
{
	return mOnPopulateCommands;
}

void aiva::layer1::GraphicPipeline::InitializeDirectX()
{
	mEngine.OnRender().connect(boost::bind(&aiva::layer1::GraphicPipeline::TickDirectX, this));
}

void aiva::layer1::GraphicPipeline::TickDirectX()
{
	auto const& fence = mEngine.GraphicHardware().Fence();
	winrt::check_bool(fence);

	auto const& commandQueue = mEngine.GraphicHardware().CommandQueue();
	winrt::check_bool(commandQueue);

	auto const& commandAllocator = mEngine.GraphicHardware().CommandAllocator();
	winrt::check_bool(commandAllocator);

	auto const& commandList = mEngine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& swapChain = mEngine.GraphicHardware().SwapChain();
	winrt::check_bool(swapChain);

	WaitFrame(fence, mEngine.Tick());
	PresentFrame(swapChain, mEngine.GraphicHardware().IsTearingAllowed());
	ResetCommandList(commandAllocator, commandList);
	OnPopulateCommands()();
	CloseCommandList(commandList);
	ExecuteCommandList(commandQueue, commandList);
	ExecuteSignalForFrame(commandQueue, fence, mEngine.Tick() + 1);
}

void aiva::layer1::GraphicPipeline::TerminateDirectX()
{
	mEngine.OnRender().disconnect(boost::bind(&aiva::layer1::GraphicPipeline::TickDirectX, this));
}

void aiva::layer1::GraphicPipeline::WaitFrame(winrt::com_ptr<ID3D12Fence1> const& fence, uint64_t const desiredFrame)
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

void aiva::layer1::GraphicPipeline::PresentFrame(winrt::com_ptr<IDXGISwapChain4> const& swapChain, bool const isTearingAllowed)
{
	winrt::check_bool(swapChain);

	UINT const syncInterval = SWAP_CHAIN_VSYNC_ENABLED ? 1 : 0;
	UINT const presentFlags = isTearingAllowed && !SWAP_CHAIN_VSYNC_ENABLED ? DXGI_PRESENT_ALLOW_TEARING : 0;

	winrt::check_hresult(swapChain->Present(syncInterval, presentFlags));
}

void aiva::layer1::GraphicPipeline::ResetCommandList(winrt::com_ptr<ID3D12CommandAllocator> const& commandAllocator, winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList)
{
	winrt::check_bool(commandAllocator);
	winrt::check_bool(commandList);

	winrt::check_hresult(commandAllocator->Reset());
	winrt::check_hresult(commandList->Reset(commandAllocator.get(), nullptr));
}

void aiva::layer1::GraphicPipeline::CloseCommandList(winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList)
{
	winrt::check_bool(commandList);

	commandList->Close();
}

void aiva::layer1::GraphicPipeline::ExecuteCommandList(winrt::com_ptr<ID3D12CommandQueue> const& commandQueue, winrt::com_ptr<ID3D12GraphicsCommandList6> const& commandList)
{
	winrt::check_bool(commandQueue);
	winrt::check_bool(commandList);

	std::array<ID3D12CommandList*, 1> const commandsList = { commandList.get() };
	commandQueue->ExecuteCommandLists(1, commandsList.data());
}

void aiva::layer1::GraphicPipeline::ExecuteSignalForFrame(winrt::com_ptr<ID3D12CommandQueue> const& commandQueue, winrt::com_ptr<ID3D12Fence1> const& fence, uint64_t frame)
{
	winrt::check_bool(commandQueue);
	winrt::check_bool(fence);
	
	winrt::check_hresult(commandQueue->Signal(fence.get(), frame));
}
