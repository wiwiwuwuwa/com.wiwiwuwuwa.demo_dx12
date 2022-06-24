#include "pch.h"
#include "renderer.h"

aiva::Renderer::Renderer()
{
	EnableDebugLayer();
}

void aiva::Renderer::EnableDebugLayer()
{
#if defined(_DEBUG)
	winrt::com_ptr<ID3D12Debug> debug{};
	winrt::check_hresult(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)));
	debug->EnableDebugLayer();
#endif
}
