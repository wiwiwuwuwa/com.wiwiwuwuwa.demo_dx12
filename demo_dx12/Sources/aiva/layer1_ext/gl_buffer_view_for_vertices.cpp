#include <pch.h>
#include <aiva/layer1_ext/gl_buffer_view_for_vertices.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/shader_consts.h>
#include <aiva/layer1_ext/shader_meta_for_semantic.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	GlBufferViewForVertices::GlBufferViewForVertices(tinygltf::Model const& gltf, std::size_t const meshIndex, std::size_t const primitiveIndex, aiva::layer1::Engine& engine)
	{
		auto const& mesh = gltf.meshes.at(meshIndex);
		auto const& primitive = mesh.primitives.at(primitiveIndex);
		auto const& attributes = primitive.attributes;

		auto const attributesSorter = [](auto const& a, auto const& b) { return (*a) < (*b); };
		auto sortedAttributes = std::map<std::shared_ptr<ShaderMetaForSemantic>, int, decltype(attributesSorter)>{ attributesSorter };

		std::transform(std::cbegin(attributes), std::cend(attributes), std::inserter(sortedAttributes, std::end(sortedAttributes)), [&](auto const& pair) { return std::make_pair(NewObject<ShaderMetaForSemantic>(pair.first, ShaderParsingRulesForSemantic::GLTF, engine), pair.second); });
		std::transform(std::cbegin(sortedAttributes), std::cend(sortedAttributes), std::back_inserter(mBufferViews), [&](auto const& pair) { return GlBufferView{ gltf, static_cast<std::size_t>(pair.second) }; });

		Asserts::CheckBool(!std::empty(mBufferViews), "Buffers views are not valid");

		mNumOfElements = mBufferViews.front().NumOfElements();
		std::transform(std::cbegin(mBufferViews), std::cend(mBufferViews), std::back_inserter(mTypeOfElement), [](auto const& view) { return view.TypeOfElement(); });
		mSizeOfElement = std::accumulate(std::cbegin(mBufferViews), std::cend(mBufferViews), decltype(mSizeOfElement){}, [](auto const& a, auto const& b) { return a + b.SizeOfElement(); });
		mSizeOfData = mSizeOfElement * mNumOfElements;
	}

	std::vector<std::byte> GlBufferViewForVertices::GetData() const
	{
		auto binaryForData = std::vector<std::byte>{};

		for (auto i = std::size_t{}; i < mNumOfElements; i++)
		{
			auto const binaryForElement = GetElement(i);
			binaryForData.insert(std::cend(binaryForData), std::cbegin(binaryForElement), std::cend(binaryForElement));
		}

		return binaryForData;
	}

	std::vector<std::byte> GlBufferViewForVertices::GetElement(std::size_t const index) const
	{
		auto binaryForData = std::vector<std::byte>{};

		for (auto const& bufferView : mBufferViews)
		{
			auto const binaryForElement = bufferView.GetElement(index);
			binaryForData.insert(std::cend(binaryForData), std::cbegin(binaryForElement), std::cend(binaryForElement));
		}

		return binaryForData;
	}
}
