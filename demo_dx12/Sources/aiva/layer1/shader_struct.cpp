#include <pch.h>
#include <aiva/layer1/shader_struct.h>

#include <aiva/layer1/shader_consts.h>
#include <aiva/utils/math_utils.h>

aiva::layer1::ShaderStruct::ShaderStruct()
{

}

aiva::layer1::ShaderStruct::~ShaderStruct()
{

}

std::vector<std::byte> aiva::layer1::ShaderStruct::SerializeToBinary() const
{
	std::vector<decltype(mValues)::const_iterator> iterators{};
	for (auto iterator = mValues.cbegin(); iterator != mValues.cend(); iterator++) iterators.push_back(iterator);

	std::sort(iterators.begin(), iterators.end(), [](auto const& a, auto const& b) { return a->first < b->first; });

	std::vector<std::byte> binaryData{};
	for (auto const& iterator : iterators)
	{
		auto const& valueObject = iterator->second;
		aiva::utils::Asserts::CheckBool(valueObject);

		auto const& valueBinary = valueObject->SerializeToBinary();
		if (valueBinary.size() <= 0) continue;

		auto const pointer = aiva::utils::MathUtils::Align(binaryData.size(), valueBinary.size(), ShaderConsts::VECTOR_SIZE);

		binaryData.resize(pointer + valueBinary.size());
		aiva::utils::Asserts::CheckBool(memcpy_s(binaryData.data() + pointer, valueBinary.size(), valueBinary.data(), valueBinary.size()) == 0);
	}

	{
		auto const binarySize = aiva::utils::MathUtils::Align(binaryData.size(), ShaderConsts::VECTOR_SIZE);
		binaryData.resize(binarySize);
	}

	return binaryData;
}

bool aiva::layer1::ShaderStruct::HasSameFields(ShaderStruct const& other) const
{
	if (mValues.size() != other.mValues.size())
	{
		return false;
	}

	return std::all_of(mValues.cbegin(), mValues.cend(), [&](auto const& pair) { return other.mValues.find(pair.first) != other.mValues.end(); });
}
