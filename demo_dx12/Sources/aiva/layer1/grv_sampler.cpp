#include <pch.h>
#include <aiva/layer1/grv_sampler.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	GrvSampler::GrvSampler(EngineType const& engine) : AGraphicResourceView{ engine }
	{

	}

	GrvSampler::~GrvSampler()
	{

	}

	EDescriptorHeapType GrvSampler::HeapType() const
	{
		return EDescriptorHeapType::Sampler;
	}

	EResourceViewType GrvSampler::ViewType() const
	{
		return EResourceViewType::Sampler;
	}

	void GrvSampler::CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination)
	{
		auto const& device = Engine().GraphicHardware().Device();
		winrt::check_bool(device);

		auto viewDesc = D3D12_SAMPLER_DESC{};
		viewDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		viewDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		viewDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		viewDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//viewDesc.MipLODBias = {};
		//viewDesc.MaxAnisotropy = 1;
		//viewDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		viewDesc.MinLOD = {};
		viewDesc.MaxLOD = std::numeric_limits<std::float_t>::infinity();

		device->CreateSampler(&viewDesc, destination);
	}

	std::vector<D3D12_RESOURCE_BARRIER> GrvSampler::CreateDirectxBarriers(bool const active)
	{
		return {};
	}
}
