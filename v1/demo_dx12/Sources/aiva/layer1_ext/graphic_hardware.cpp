#include <pch.h>
#include <aiva/layer1_ext/graphic_hardware.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1_ext/ro_texture_2d.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	GraphicHardware::GraphicHardware(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitScreenParamsFields();
	}

	GraphicHardware::~GraphicHardware()
	{
		ShutScreenParamsFields();
	}

	ID3D12GraphicsCommandList6& GraphicHardware::CommandList() const
	{
		auto const& commandList = mEngine.GraphicHardware().CommandList();
		Asserts::CheckBool(commandList, "Command list is not valid");

		return *commandList;
	}

	ID3D12Device9& GraphicHardware::Device() const
	{
		auto const& device = mEngine.GraphicHardware().Device();
		Asserts::CheckBool(device, "Device is not valid");

		return *device;
	}

	std::shared_ptr<RoTexture2D> const& GraphicHardware::ScreenTexture() const
	{
		auto const& swapChain = Engine().GraphicHardware().SwapChain();
		Asserts::CheckBool(swapChain, "Swap chain is not valid");

		auto const textureIndex = decltype(mScreenTextures)::size_type{ swapChain->GetCurrentBackBufferIndex() };
		Asserts::CheckBool(textureIndex >= decltype(textureIndex){} && textureIndex < std::size(mScreenTextures), "Texture index is not valid");

		auto const& screenTexture = mScreenTextures.at(textureIndex);
		Asserts::CheckBool(screenTexture, "Screen texture is not valid");

		return screenTexture;
	}

	glm::size2 GraphicHardware::ScreenSize() const
	{
		auto const& screenTexture = ScreenTexture();
		Asserts::CheckBool(screenTexture, "Screen texture is not valid");

		auto const screenSize = glm::size2{ screenTexture->Width(), screenTexture->Height() };
		Asserts::CheckBool(screenSize.x > decltype(screenSize)::value_type{}, "Screen size width is not valid");
		Asserts::CheckBool(screenSize.y > decltype(screenSize)::value_type{}, "Screen size height is not valid");

		return screenSize;
	}

	void GraphicHardware::InitScreenParamsFields()
	{
		mScreenTextures = {};

		auto const& swapChain = Engine().GraphicHardware().SwapChain();
		Asserts::CheckBool(swapChain, "Swap chain is not valid");

		auto swapDesc = DXGI_SWAP_CHAIN_DESC1{};
		winrt::check_hresult(swapChain->GetDesc1(&swapDesc));

		for (auto i = std::size_t{}; i < std::size_t{ swapDesc.BufferCount }; i++)
		{
			auto resource = winrt::com_ptr<ID3D12Resource>{};
			winrt::check_hresult(swapChain->GetBuffer(i, IID_PPV_ARGS(&resource)));
			Asserts::CheckBool(resource, "Resource is not valid");

			auto const& texture2D = mScreenTextures.emplace_back(NewObject<RoTexture2D>(resource, Engine()));
			Asserts::CheckBool(texture2D, "Texture 2D is not valid");
		}
	}

	void GraphicHardware::ShutScreenParamsFields()
	{
		mScreenTextures = {};
	}
}
