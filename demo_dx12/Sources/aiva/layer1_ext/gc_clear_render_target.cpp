#include <pch.h>
#include <aiva/layer1_ext/gc_clear_render_target.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/graphic_hardware.h>
#include <aiva/layer1_ext/rt_render_identity.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	GcClearRenderTarget::GcClearRenderTarget(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		mTarget = NewObject<decltype(mTarget)::element_type>(engine);
		Asserts::CheckBool(mTarget, "Target is not valid");

		OnExec().AttachListener(&ThisType::GraphicCommand_OnExec, this);
	}

	GcClearRenderTarget::~GcClearRenderTarget()
	{
		OnExec().RemoveListener(&ThisType::GraphicCommand_OnExec, this);

		Asserts::CheckBool(mTarget, "Target is not valid");
		mTarget = {};
	}

	void GcClearRenderTarget::GraphicCommand_OnExec()
	{
		ExecuteResourceBarrier();
		ExecuteClearRenderTargetView();
	}

	void GcClearRenderTarget::ExecuteResourceBarrier()
	{
		Asserts::CheckBool(mTarget, "Target is not valid");
		Asserts::CheckBool(!mTarget->Empty(), "Target doesn't contain resource");

		auto const resourceBarriers = mTarget->TransiteResourceBarriers(UtBarriersFlags::Setup);
		if (std::empty(resourceBarriers))
		{
			return;
		}

		mEngine.GraphicHardwareExt().CommandList().ResourceBarrier(std::size(resourceBarriers), std::data(resourceBarriers));
	}

	void GcClearRenderTarget::ExecuteClearRenderTargetView()
	{
		Asserts::CheckBool(mTarget, "Target is not valid");
		Asserts::CheckBool(!mTarget->Empty(), "Target doesn't contain resource");

		auto const handle = mTarget->NativeHandleCPU();
		auto const color = glm::value_ptr(mColor);

		mEngine.GraphicHardwareExt().CommandList().ClearRenderTargetView(handle, color, {}, {});
	}
}
