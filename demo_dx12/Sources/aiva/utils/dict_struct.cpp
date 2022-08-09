#include <pch.h>
#include <aiva/utils/dict_struct.h>

#include <aiva/layer1/shader_consts.h>
#include <aiva/utils/align_field_info.h>
#include <aiva/utils/align_struct_info.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/i_boxed_value.h>
#include <aiva/utils/math_utils.h>
#include <aiva/utils/object_utils.h>

aiva::utils::DictStruct::DictStruct() : AObject{}, IObjectChangeable{}
{

}

aiva::utils::DictStruct::~DictStruct()
{

}

aiva::utils::DictStruct::FieldElemType aiva::utils::DictStruct::FieldBoxed(std::string const& name) const
{
	auto const iter = mFieldsBoxed.find(name);
	if (iter == mFieldsBoxed.end()) return {};

	auto const& field = iter->second;
	Asserts::CheckBool(field, "Field is not valid");

	return field;
}

aiva::utils::DictStruct& aiva::utils::DictStruct::FieldBoxed(std::string const& name, FieldElemType const& fieldBoxed)
{
	auto const previousField = FieldBoxed(name);
	if (previousField)
	{
		previousField->OnChanged().disconnect(boost::bind(&DictStruct::FieldBoxed_OnChanged, this));
		mFieldsBoxed.erase(name);
	}

	auto const& desiredField = fieldBoxed;
	if (desiredField)
	{
		desiredField->OnChanged().connect(boost::bind(&DictStruct::FieldBoxed_OnChanged, this));
		mFieldsBoxed.insert_or_assign(name, desiredField);
	}

	OnChanged()();
	return *this;
}

aiva::utils::DictStruct::FieldDictType const& aiva::utils::DictStruct::FieldsBoxed() const
{
	return mFieldsBoxed;
}

void aiva::utils::DictStruct::FieldBoxed_OnChanged()
{
	OnChanged()();
}

aiva::utils::DictStruct::AlignInfoType aiva::utils::DictStruct::CreateAlignInfo(AlignModeType const mode) const
{
	switch (mode)
	{
	case AlignModeType::MaxSpeed:
		return CreateAlignInfo_MaxSpeed();

	default:
		Asserts::CheckBool(false, "Unsupported AlignMode type");
		return {};
	}
}

aiva::utils::DictStruct::AlignInfoType aiva::utils::DictStruct::CreateAlignInfo_MaxSpeed() const
{
	auto iterators = std::vector<FieldDictType::const_iterator>{};
	for (auto iterator = std::cbegin(FieldsBoxed()); iterator != std::cend(FieldsBoxed()); iterator++) iterators.push_back(iterator);
	std::sort(std::begin(iterators), std::end(iterators), [](auto const& a, auto const& b) { return a->first < b->first; });

	auto structInfo = NewObject<AlignStructInfo>();
	Asserts::CheckBool(structInfo, "Struct info is not valid");

	for (auto const& iterator : iterators)
	{
		auto const& fieldObject = iterator->second;
		Asserts::CheckBool(fieldObject, "Field object is not valid");

		auto const fieldBinary = fieldObject->Binary();
		Asserts::CheckBool(!std::empty(fieldBinary), "Field binary is not valid");

		auto const pointer = MathUtils::Align(structInfo->Size(), std::size(fieldBinary), aiva::layer1::ShaderConsts::VECTOR_SIZE);
		structInfo->Size(pointer + std::size(fieldBinary));

		auto fieldInfo = NewObject<AlignFieldInfo>();
		Asserts::CheckBool(fieldInfo, "Field info is not valid");

		fieldInfo->Offset(pointer);
		structInfo->Field(iterator->first, fieldInfo);
	}

	{
		auto const pointer = MathUtils::Align(structInfo->Size(), aiva::layer1::ShaderConsts::VECTOR_SIZE);
		structInfo->Size(pointer);
	}

	return structInfo;
}

std::vector<std::byte> aiva::utils::DictStruct::SerializeToBinary(AlignModeType const mode) const
{
	auto const structInfo = CreateAlignInfo(mode);
	Asserts::CheckBool(structInfo, "Struct info is not valid");
	Asserts::CheckBool(structInfo->Size() > 0, "Struct info size is not valid");

	auto structBinary = std::vector<std::byte>{ structInfo->Size() };

	for (auto const& pair : structInfo->Fields())
	{
		auto const& fieldName = pair.first;
		auto const& fieldInfo = pair.second;

		Asserts::CheckBool(fieldInfo, "Field info is not valid");
		Asserts::CheckBool(fieldInfo->Offset() >= 0 && fieldInfo->Offset() < std::size(structBinary), "Field info offset is out of range");

		auto const fieldObject = FieldBoxed(pair.first);
		Asserts::CheckBool(fieldObject, "Field info doesn't match struct field");

		auto const fieldBinary = fieldObject->Binary();
		Asserts::CheckBool(!std::empty(fieldBinary), "Field binary is not valid");
		Asserts::CheckBool(fieldInfo->Offset() + std::size(fieldBinary) <= std::size(structBinary), "Field binary is out of range");

		Asserts::CheckBool(memcpy_s(std::data(structBinary) + fieldInfo->Offset(), std::size(fieldBinary), std::data(fieldBinary), std::size(fieldBinary)) == 0,"Failed to memcpy_s");
	}

	return structBinary;
}

aiva::utils::DictStruct& aiva::utils::DictStruct::DeserealizeFromBinary(boost::span<const std::byte> const& binary, AlignModeType const mode)
{
	Asserts::CheckBool(!std::empty(binary), "Binary data is not valid");

	auto const structInfo = CreateAlignInfo(mode);
	Asserts::CheckBool(structInfo, "Struct info is not valid");
	Asserts::CheckBool(structInfo->Size() > 0, "Struct info size is not valid");
	Asserts::CheckBool(std::size(binary) >= structInfo->Size(), "Binary data size is not valid");

	for (auto const& pair : structInfo->Fields())
	{
		auto const& fieldName = pair.first;
		auto const& fieldInfo = pair.second;

		Asserts::CheckBool(fieldInfo, "Field info is not valid");
		Asserts::CheckBool(fieldInfo->Offset() >= 0 && fieldInfo->Offset() < std::size(binary), "Field info offset is out of range");

		auto const fieldObject = FieldBoxed(pair.first);
		Asserts::CheckBool(fieldObject, "Field info doesn't match struct field");

		auto const fieldBinary = fieldObject->Binary();
		Asserts::CheckBool(!std::empty(fieldBinary), "Field binary is not valid");
		Asserts::CheckBool(fieldInfo->Offset() + std::size(fieldBinary) <= std::size(binary), "Field binary is out of range");

		Asserts::CheckBool(memcpy_s(std::data(fieldBinary), std::size(fieldBinary), std::data(binary) + fieldInfo->Offset(), std::size(fieldBinary)) == 0, "Failed to memcpy_s");
	}

	return *this;
}
