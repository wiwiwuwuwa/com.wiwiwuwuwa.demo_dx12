#include <pch.h>
#include <aiva/layer1/gca_set_render_target.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/grv_rtv_to_texture_2d.h>
#include <aiva/layer1/res_view_desc.h>
#include <aiva/layer1/res_view_desc_utils.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	void GcaSetRenderTarget::Execute(Engine const& engine) const
	{
		ExecuteResourceBarrier(engine);
		ExecuteSetRenderTargets(engine);
	}

	void GcaSetRenderTarget::ExecuteResourceBarrier(Engine const& engine) const
	{
		auto const& commandList = engine.GraphicHardware().CommandList();
		winrt::check_bool(commandList);

		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		for (auto const& rt : RTs)
		{
			Asserts::CheckBool(ResViewDescUtils::IsValid(rt), "RT is not valid");

			auto const rtView = ResViewDescUtils::GetView(rt);
			Asserts::CheckBool(rtView, "RT view is not valid");

			auto const rtBarriers = rtView->CreateDirectxBarriers(true);
			std::copy(std::cbegin(rtBarriers), std::cend(rtBarriers), std::back_inserter(barriers));
		}

		for (auto const& ds : DSs)
		{
			Asserts::CheckBool(ResViewDescUtils::IsValid(ds), "DS is not valid");

			auto const dsView = ResViewDescUtils::GetView(ds);
			Asserts::CheckBool(dsView, "DS view is not valid");

			auto const dsBarriers = dsView->CreateDirectxBarriers(true);
			std::copy(std::cbegin(dsBarriers), std::cend(dsBarriers), std::back_inserter(barriers));
		}

		if (std::empty(barriers))
		{
			return;
		}

		commandList->ResourceBarrier(std::size(barriers), std::data(barriers));
	}

	void GcaSetRenderTarget::ExecuteSetRenderTargets(Engine const& engine) const
	{
		auto const& commandList = engine.GraphicHardware().CommandList();
		winrt::check_bool(commandList);

		auto rtHandles = std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>{};
		for (auto const& rt : RTs)
		{
			Asserts::CheckBool(ResViewDescUtils::IsValid(rt), "RT is not valid");

			auto const rtHandle = ResViewDescUtils::GetHandle(rt);
			Asserts::CheckBool(rtHandle, "RT handle is not valid");

			rtHandles.emplace_back(*rtHandle);
		}

		auto dsHandles = std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>{};
		for (auto const& ds : DSs)
		{
			Asserts::CheckBool(ResViewDescUtils::IsValid(ds), "DS is not valid");

			auto const dsHandle = ResViewDescUtils::GetHandle(ds);
			Asserts::CheckBool(dsHandle, "DS handle is not valid");

			dsHandles.emplace_back(*dsHandle);
		}

		auto const rtSize = std::size(rtHandles);
		auto const rtPointer = rtSize ? std::data(rtHandles) : nullptr;
		auto const dsSize = std::size(dsHandles);
		auto const dsPointer = dsSize ? std::data(dsHandles) : nullptr;

		commandList->OMSetRenderTargets(rtSize, rtPointer, false, dsPointer);
	}
}
