#include <pch.h>
#include <aiva/layer1_ext/gc_set_scissor_rects.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/graphic_hardware.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	GcSetScissorRects::GcSetScissorRects(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		OnExec().AttachListener(&ThisType::GraphicCommand_OnExec, this);
	}

	GcSetScissorRects::~GcSetScissorRects()
	{
		OnExec().RemoveListener(&ThisType::GraphicCommand_OnExec, this);
	}

	void GcSetScissorRects::GraphicCommand_OnExec()
	{
		ExecuteRSSetScissorRects();
	}

	void GcSetScissorRects::ExecuteRSSetScissorRects()
	{
		auto rect = D3D12_RECT{};
		rect.left = mRect.x;
		rect.top = mRect.y;
		rect.right = mRect.z;
		rect.bottom = mRect.w;

		mEngine.GraphicHardwareExt().CommandList().RSSetScissorRects(1, &rect);
	}
}
