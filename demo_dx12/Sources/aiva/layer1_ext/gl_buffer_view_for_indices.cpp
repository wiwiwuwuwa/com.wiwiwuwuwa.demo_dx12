#include <pch.h>
#include <aiva/layer1_ext/gl_buffer_view_for_indices.h>

namespace aiva::layer1_ext
{
	GlBufferViewForIndices::GlBufferViewForIndices(tinygltf::Model const& gltf, std::size_t const meshIndex, std::size_t const primitiveIndex)
	{
		auto const& mesh = gltf.meshes.at(meshIndex);
		auto const& primitive = mesh.primitives.at(primitiveIndex);
		auto const accessorIndex = primitive.indices;

		mBufferView = decltype(mBufferView){ gltf, static_cast<std::size_t>(accessorIndex) };
	}

	std::size_t GlBufferViewForIndices::SizeOfData() const
	{
		return mBufferView.SizeOfData();
	}

	std::vector<std::byte> GlBufferViewForIndices::GetData() const
	{
		return mBufferView.GetData();
	}

	std::size_t GlBufferViewForIndices::NumOfElements() const
	{
		return mBufferView.NumOfElements();
	}

	aiva::utils_ext::EValueType GlBufferViewForIndices::TypeOfElement() const
	{
		return mBufferView.TypeOfElement();
	}

	std::size_t GlBufferViewForIndices::SizeOfElement() const
	{
		return mBufferView.SizeOfElement();
	}

	std::vector<std::byte> GlBufferViewForIndices::GetElement(std::size_t const index) const
	{
		return mBufferView.GetElement(index);
	}
}
