#include "pch.h"
#include "renderer.h"

aiva::Renderer::Renderer()
{
	EnableDebugLayer();
	CreateInfoQueue(CreateDevice(CreateAdapter(CreateFactory())));
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
	winrt::check_pointer(factory.get());

	winrt::com_ptr<IDXGIAdapter> basicAdapter{};
	winrt::check_hresult(factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&basicAdapter)));

	winrt::com_ptr<IDXGIAdapter4> specificAdapter{};
	basicAdapter.as(specificAdapter);

	return specificAdapter;
}

winrt::com_ptr<ID3D12Device9> aiva::Renderer::CreateDevice(winrt::com_ptr<IDXGIAdapter4> const& adapter)
{
	winrt::check_pointer(adapter.get());

	winrt::com_ptr<ID3D12Device> basicDevice{};
	winrt::check_hresult(D3D12CreateDevice(adapter.get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&basicDevice)));

	winrt::com_ptr<ID3D12Device9> specificDevice{};
	basicDevice.as(specificDevice);

	return specificDevice;
}

#if defined(_DEBUG)
winrt::com_ptr<ID3D12InfoQueue1> aiva::Renderer::CreateInfoQueue(winrt::com_ptr<ID3D12Device9> const& device)
{
	winrt::check_pointer(device.get());

	winrt::com_ptr<ID3D12InfoQueue1> infoQueue{};
	device.as(infoQueue);

	return infoQueue;
}
#endif
