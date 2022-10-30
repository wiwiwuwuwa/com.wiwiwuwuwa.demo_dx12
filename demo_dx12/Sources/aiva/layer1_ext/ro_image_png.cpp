#include <pch.h>
#include <aiva/layer1_ext/ro_image_png.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	RoImagePng::RoImagePng(aiva::layer1::Engine& engine) : mEngine{ engine }
	{

	}

	RoImagePng::~RoImagePng()
	{
		if (Data())
		{
			stbi_image_free(Data());
			Data() = {};
		}
	}

	void RoImagePng::DeserealizeFromBinary(boost::span<const std::byte> const& resourceBin)
	{
		Asserts::CheckBool(!std::empty(resourceBin), "Resource bin is not valid");

		if (Data())
		{
			stbi_image_free(Data());
			Data() = {};
		}

		auto const inBuffer = reinterpret_cast<stbi_uc const*>(std::data(resourceBin));
		auto const len = static_cast<int>(std::size(resourceBin));
		auto x = int{};
		auto y = int{};
		auto c = int{};

		auto const outBuffer = stbi_load_from_memory(inBuffer, len, &x, &y, &c, {});
		Asserts::CheckBool(outBuffer, "Out buffer is not valid");

		Width() = static_cast<std::size_t>(x);
		Height() = static_cast<std::size_t>(y);
		Channels() = static_cast<std::size_t>(c);
		Data() = reinterpret_cast<std::byte*>(outBuffer);

		OnUpdate().Broadcast();
	}
}
