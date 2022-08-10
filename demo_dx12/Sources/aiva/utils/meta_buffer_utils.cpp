#include <pch.h>
#include <aiva/utils/meta_buffer_utils.h>

#include <aiva/utils/asserts.h>
#include <aiva/utils/dict_buffer.h>
#include <aiva/utils/meta_buffer.h>
#include <aiva/utils/meta_struct.h>
#include <aiva/utils/meta_struct_utils.h>
#include <aiva/utils/object_utils.h>

aiva::utils::MetaBufferUtils::MetaBufferPointerType aiva::utils::MetaBufferUtils::GenerateFrom(DictBufferPointerType const& dictBuffer)
{
	Asserts::CheckBool(dictBuffer, "Dict buffer is not valid");
	Asserts::CheckBool(dictBuffer->Layout(), "Dict buffer layout is not valid");

	auto const metaBuffer = NewObject<MetaBufferElementType>();
	Asserts::CheckBool(metaBuffer, "Meta buffer is not valid");

	metaBuffer->Layout(MetaStructUtils::Copy(dictBuffer->Layout()));
	metaBuffer->Count(dictBuffer->Num());

	return metaBuffer;
}

aiva::utils::MetaBufferUtils::MetaBufferPointerType aiva::utils::MetaBufferUtils::GenerateFrom(LayoutBufferPointerType const& layoutBuffer)
{
	Asserts::CheckBool(false, "TODO: Impl - MetaBufferUtils::GenerateFrom(LayoutBufferPointerType)");
	return {};
}
