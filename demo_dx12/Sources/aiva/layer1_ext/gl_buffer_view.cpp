#include <pch.h>
#include <aiva/layer1_ext/gl_buffer_view.h>

#include <aiva/layer1_ext/gl_type_desc.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/e_value_type_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	GlBufferView::GlBufferView(tinygltf::Model const& gltf, std::size_t const accessorIndex)
		: mGLTF{ &gltf }, mAccessorIndex{ accessorIndex }
	{
		auto const& accessor = gltf.accessors.at(mAccessorIndex);

		mNumOfElements = accessor.count;
		mTypeOfElement = EValueTypeUtils::ParseFromGLTF({ static_cast<std::size_t>(accessor.type), static_cast<std::size_t>(accessor.componentType) });
		mSizeOfElement = EValueTypeUtils::SizeOf(mTypeOfElement);
		mSizeOfData = mSizeOfElement * mNumOfElements;
	}

	std::vector<std::byte> GlBufferView::GetData() const
	{
		auto binaryForData = std::vector<std::byte>{};

		for (auto i = std::size_t{}; i < mNumOfElements; i++)
		{
			auto const binaryForElement = GetElement(i);
			binaryForData.insert(std::cend(binaryForData), std::cbegin(binaryForElement), std::cend(binaryForElement));
		}

		return binaryForData;
	}

	std::vector<std::byte> GlBufferView::GetElement(std::size_t const index) const
	{
		Asserts::CheckBool(mGLTF, "GLTF is not valid");

		auto const& accessor = (*mGLTF).accessors.at(mAccessorIndex);
		auto const& bufferView = (*mGLTF).bufferViews.at(accessor.bufferView);
		auto const& buffer = (*mGLTF).buffers.at(bufferView.buffer);

		auto const sizeOfElement = mSizeOfElement;
		auto const strideOfElement = std::max(bufferView.byteStride, sizeOfElement);

		auto binary = boost::as_bytes(boost::span{ buffer.data });
		binary = binary.subspan(bufferView.byteOffset, bufferView.byteLength);
		binary = binary.last(std::size(binary) - accessor.byteOffset);
		binary = binary.subspan(strideOfElement * index, sizeOfElement);

		return std::vector<std::byte>{ std::cbegin(binary), std::cend(binary) };
	}
}
