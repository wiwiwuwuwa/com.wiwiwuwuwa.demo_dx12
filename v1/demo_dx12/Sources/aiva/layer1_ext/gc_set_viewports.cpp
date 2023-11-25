#include <pch.h>
#include <aiva/layer1_ext/gc_set_viewports.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/graphic_hardware.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	GcSetViewports::GcSetViewports(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		OnExec().AttachListener(&ThisType::GraphicCommand_OnExec, this);
	}

	GcSetViewports::~GcSetViewports()
	{
		OnExec().RemoveListener(&ThisType::GraphicCommand_OnExec, this);
	}

	void GcSetViewports::GraphicCommand_OnExec()
	{
		ExecuteRSSetViewports();
	}

	void GcSetViewports::ExecuteRSSetViewports()
	{
		auto viewport = D3D12_VIEWPORT{};
		viewport.TopLeftX = mRect.x;
		viewport.TopLeftY = mRect.y;
		viewport.Width = mRect.z - mRect.x;
		viewport.Height = mRect.w - mRect.y;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		mEngine.GraphicHardwareExt().CommandList().RSSetViewports(1, &viewport);
	}
}
