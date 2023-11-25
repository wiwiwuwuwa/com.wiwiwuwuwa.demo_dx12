#include <pch.h>
#include <aiva/layer1_ext/ro_image_png.h>

#include <aiva/layer1/e_resource_buffer_format.h>
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

	}

	void RoImagePng::DeserealizeFromBinary(boost::span<const std::byte> const& resourceBin)
	{
		static auto const DEFAULT_NUMBER_OF_CHANNELS = aiva::layer1::ToPngChannels(aiva::layer1::EResourceBufferFormat::R8G8B8A8_UNORM);

		Asserts::CheckBool(!std::empty(resourceBin), "Resource bin is not valid");

		auto const inBuffer = reinterpret_cast<stbi_uc const*>(std::data(resourceBin));
		auto const inLength = static_cast<int>(std::size(resourceBin));

		auto outX = int{};
		auto outY = int{};
		auto outC = int{};
		auto const outBuffer = stbi_load_from_memory(inBuffer, inLength, &outX, &outY, &outC, DEFAULT_NUMBER_OF_CHANNELS);

		Asserts::CheckBool(outBuffer, "Out buffer is not valid");
		Asserts::CheckBool(outX > decltype(outX) {}, "Out x is not valid");
		Asserts::CheckBool(outY > decltype(outY) {}, "Out y is not valid");
		Asserts::CheckBool(outC > decltype(outC) {}, "Out c is not valid");

		mWidth = static_cast<decltype(mWidth)>(outX);
		mHeight = static_cast<decltype(mHeight)>(outY);
		mChannels = static_cast<decltype(mChannels)>(DEFAULT_NUMBER_OF_CHANNELS);
		mData = decltype(mData) { reinterpret_cast<std::add_pointer_t<decltype(mData)::element_type>>(outBuffer) };

		Asserts::CheckBool(mWidth > decltype(mWidth) {}, "Width is not valid");
		Asserts::CheckBool(mHeight > decltype(mHeight) {}, "Height is not valid");
		Asserts::CheckBool(mChannels > decltype(mChannels) {}, "Channels is not valid");
		Asserts::CheckBool(mData, "Data is not valid");

		DirtyFlags().Dirty();
	}

	boost::span<const std::byte> RoImagePng::Span() const
	{
		auto const ptr = mData.get();
		auto const num = mWidth * mHeight * mChannels;
		return { ptr, num };
	}
}
