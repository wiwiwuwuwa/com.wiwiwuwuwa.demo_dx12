#include <pch.h>
#include <aiva/utils/layout_buffer_utils.h>

#include <aiva/layer1/shader_consts.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/layout_buffer.h>
#include <aiva/utils/layout_buffer_utils.h>
#include <aiva/utils/layout_struct.h>
#include <aiva/utils/layout_struct_utils.h>
#include <aiva/utils/math_utils.h>
#include <aiva/utils/meta_buffer.h>
#include <aiva/utils/meta_buffer_utils.h>
#include <aiva/utils/meta_struct.h>
#include <aiva/utils/meta_struct_utils.h>
#include <aiva/utils/object_utils.h>

aiva::utils::LayoutBufferUtils::LayoutBufferPointerType aiva::utils::LayoutBufferUtils::GenerateFrom(DictBufferPointerType const& dictBuffer)
{
	Asserts::CheckBool(dictBuffer, "Dict buffer is not valid");

	auto const metaBuffer = MetaBufferUtils::GenerateFrom(dictBuffer);
	Asserts::CheckBool(metaBuffer, "Meta buffer is not valid");

	auto const layoutBuffer = GenerateFrom(metaBuffer);
	Asserts::CheckBool(layoutBuffer, "Layout buffer is not valid");

	return layoutBuffer;
}

aiva::utils::LayoutBufferUtils::LayoutBufferPointerType aiva::utils::LayoutBufferUtils::GenerateFrom(MetaBufferPointerType const& metaBuffer)
{
	Asserts::CheckBool(metaBuffer, "Meta buffer is not valid");
	Asserts::CheckBool(metaBuffer->Layout(), "Meta buffer layout is not valid");
	Asserts::CheckBool(metaBuffer->Count() >= 0, "Meta buffer count is not valid");

	auto const layoutBuffer = NewObject<LayoutBufferElementType>();
	Asserts::CheckBool(layoutBuffer, "Layout buffer is not valid");

	auto const layoutStruct = LayoutStructUtils::GenerateFrom(metaBuffer->Layout());
	Asserts::CheckBool(layoutStruct, "Layout struct is not valid");
	layoutBuffer->Layout(layoutStruct);

	auto const layoutStride = MathUtils::Align(layoutStruct->Size(), aiva::layer1::ShaderConsts::VECTOR_SIZE);
	layoutBuffer->Stride(layoutStride);

	auto const layoutSize = MathUtils::Align(layoutStride * metaBuffer->Count(), aiva::layer1::ShaderConsts::HEAP_SIZE);
	layoutBuffer->Size(layoutSize);

	return layoutBuffer;
}
