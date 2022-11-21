#include <pch.h>
#include <aiva/layer1_ext/gc_present.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/graphic_hardware.h>
#include <aiva/layer1_ext/ro_texture_2d.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	GcPresent::GcPresent(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		OnExec().AttachListener(&ThisType::GraphicCommand_OnExec, this);
	}

	GcPresent::~GcPresent()
	{
		OnExec().RemoveListener(&ThisType::GraphicCommand_OnExec, this);
	}

	void GcPresent::GraphicCommand_OnExec()
	{
		ExecuteResourceBarrier();
	}

	void GcPresent::ExecuteResourceBarrier()
	{
		auto const& screenTexture = mEngine.GraphicHardwareExt().ScreenTexture();
		Asserts::CheckBool(screenTexture, "Screen texture is not valid");

		auto const resourceBarriers = screenTexture->TransiteResourceBarriers({}, {});
		if (std::empty(resourceBarriers))
		{
			return;
		}

		mEngine.GraphicHardwareExt().CommandList().ResourceBarrier(std::size(resourceBarriers), std::data(resourceBarriers));
	}
}
