#pragma once
#include <pch.h>

#include <aiva/layer1/i_constant_buffer_value.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	struct ConstantPacker final
	{
	// ----------------------------------------------------
	// Main

	private:
		ConstantPacker() = delete;

	// ----------------------------------------------------
	// Constant Packing

	public:
		template <typename Iterator>
		static std::vector<std::byte> PackConstants(Iterator const first, Iterator const last);
	};
}

// --------------------------------------------------------

template <typename Iterator>
std::vector<std::byte> aiva::layer1::ConstantPacker::PackConstants(Iterator const first, Iterator const last)
{
	static constexpr std::size_t FLOAT_BOUNDARY = 4;
	static constexpr std::size_t VECTOR_BOUNDARY = FLOAT_BOUNDARY * 4;
	static constexpr std::size_t HEAP_BOUNDARY = 256;

	std::vector<Iterator> unpackedValues{};
	for (auto iter = first; iter != last; iter++) unpackedValues.emplace_back(iter);
	std::sort(unpackedValues.begin(), unpackedValues.end(), [](Iterator const& iterA, Iterator const& iterB) { return iterA->first < iterB->first; });

	std::vector<std::byte> binaryData{};
	for (auto const& unpackedValue : unpackedValues)
	{
		auto const& valuePointer = unpackedValue->second;
		aiva::utils::Asserts::CheckBool(valuePointer);

		auto const& valueData = valuePointer->GetRawData();
		aiva::utils::Asserts::CheckBool(valueData.size() > 0 && valueData.size() <= VECTOR_BOUNDARY);

		std::size_t pointer = binaryData.size();

		const bool needFixFloatBoundary = (pointer % FLOAT_BOUNDARY != 0);
		pointer = needFixFloatBoundary ? ((pointer / FLOAT_BOUNDARY + 1) * FLOAT_BOUNDARY) : pointer;

		const bool needFixVectorBoundary = (pointer / VECTOR_BOUNDARY != (pointer + valueData.size() - 1) / VECTOR_BOUNDARY);
		pointer = needFixVectorBoundary ? ((pointer / VECTOR_BOUNDARY + 1) * VECTOR_BOUNDARY) : pointer;

		binaryData.resize(pointer + valueData.size());
		std::copy(valueData.begin(), valueData.end(), binaryData.begin() + pointer);
	}

	{
		const bool needFixVectorness = (binaryData.size() % VECTOR_BOUNDARY != 0);
		const std::size_t vectornessSize = needFixVectorness ? (binaryData.size() / VECTOR_BOUNDARY + 1) * VECTOR_BOUNDARY : binaryData.size();

		binaryData.resize(vectornessSize);
	}

	{
		const bool needFixHeapness = (binaryData.size() % HEAP_BOUNDARY != 0);
		const std::size_t heapnessSize = needFixHeapness ? (binaryData.size() / HEAP_BOUNDARY + 1) * HEAP_BOUNDARY : binaryData.size();

		binaryData.resize(heapnessSize);
	}

	return binaryData;
}
