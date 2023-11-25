#include <pch.h>
#include <aiva/layer1_ext/gc_set_render_target.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/graphic_hardware.h>
#include <aiva/layer1_ext/rt_render_identity.h>
#include <aiva/layer1_ext/rt_render_identity_multiple.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	GcSetRenderTarget::GcSetRenderTarget(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		mRenderTargets = NewObject<decltype(mRenderTargets)::element_type>(engine);
		Asserts::CheckBool(mRenderTargets, "Render targets object is not valid");

		mDepthTarget = NewObject<decltype(mDepthTarget)::element_type>(engine);
		Asserts::CheckBool(mDepthTarget, "Depth target object is not valid");

		OnExec().AttachListener(&ThisType::GraphicCommand_OnExec, this);
	}

	GcSetRenderTarget::~GcSetRenderTarget()
	{
		OnExec().RemoveListener(&ThisType::GraphicCommand_OnExec, this);

		Asserts::CheckBool(mDepthTarget, "Depth target object is not valid");
		mDepthTarget = {};

		Asserts::CheckBool(mRenderTargets, "Render targets object is not valid");
		mRenderTargets = {};
	}

	void GcSetRenderTarget::GraphicCommand_OnExec()
	{
		ExecuteResourceBarrier();
		ExecuteOMSetRenderTargets();
	}

	void GcSetRenderTarget::ExecuteResourceBarrier()
	{
		auto resourceBarriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		{
			auto const& renderTargets = mRenderTargets;
			Asserts::CheckBool(renderTargets, "Render targets object is not valid");

			if (!renderTargets->Empty())
			{
				auto const renderBarriers = renderTargets->TransiteResourceBarriers(UtBarriersFlags::Setup);
				resourceBarriers.insert(std::cend(resourceBarriers), std::cbegin(renderBarriers), std::cend(renderBarriers));
			}
		}

		{
			auto const& depthTarget = mDepthTarget;
			Asserts::CheckBool(depthTarget, "Depth target object is not valid");

			if (!depthTarget->Empty())
			{
				auto const depthBarriers = depthTarget->TransiteResourceBarriers(UtBarriersFlags::Setup);
				resourceBarriers.insert(std::cend(resourceBarriers), std::cbegin(depthBarriers), std::cend(depthBarriers));
			}
		}

		if (std::empty(resourceBarriers))
		{
			return;
		}

		mEngine.GraphicHardwareExt().CommandList().ResourceBarrier(std::size(resourceBarriers), std::data(resourceBarriers));
	}

	void GcSetRenderTarget::ExecuteOMSetRenderTargets()
	{
		auto renderHandle = std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>{};
		auto renderHandleNum = std::size_t{};
		auto depthHandle = std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>{};

		{
			auto const& renderTargets = mRenderTargets;
			Asserts::CheckBool(renderTargets, "Render targets object is not valid");

			if (!renderTargets->Empty())
			{
				renderHandle = renderTargets->NativeHandleCPU();
				renderHandleNum = renderTargets->Size();
			}
		}

		{
			auto const& depthTarget = mDepthTarget;
			Asserts::CheckBool(depthTarget, "Depth target object is not valid");

			if (!depthTarget->Empty())
			{
				depthHandle = depthTarget->NativeHandleCPU();
			}
		}

		auto renderData = std::add_pointer_t<decltype(renderHandle)::value_type const>{};
		auto renderDataNum = UINT{};
		auto depthData = std::add_pointer_t<decltype(depthHandle)::value_type const>{};

		{
			auto const hasRender = renderHandle.has_value() && renderHandleNum;
			if (hasRender)
			{
				renderData = &renderHandle.value();
				renderDataNum = static_cast<decltype(renderDataNum)>(renderHandleNum);
			}
		}

		{
			auto const hasDepth = depthHandle.has_value();
			if (hasDepth)
			{
				depthData = &depthHandle.value();
			}
		}

		mEngine.GraphicHardwareExt().CommandList().OMSetRenderTargets(renderDataNum, renderData, true, depthData);
	}
}
