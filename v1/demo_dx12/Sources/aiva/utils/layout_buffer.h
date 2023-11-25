#pragma once
#include <pch.h>

#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_changeable.h>

namespace aiva::utils
{
	struct LayoutStruct;
}

namespace aiva::utils
{
	struct LayoutBuffer final : public AObject, public IObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		LayoutBuffer();

	public:
		~LayoutBuffer() override;

	// ----------------------------------------------------
	// Metadata

	public:
		using LayoutStructElementType = LayoutStruct;

		using LayoutStructPointerType = std::shared_ptr<LayoutStructElementType>;

	public:
		LayoutStructPointerType const& Layout() const;

		LayoutBuffer& Layout(LayoutStructPointerType const& layout);

	private:
		void Layout_OnChanged();

	private:
		LayoutStructPointerType mLayout{};

	// --------------------------------

	public:
		std::size_t Stride() const;

		LayoutBuffer& Stride(std::size_t const stride);

	private:
		std::size_t mStride{};

	// --------------------------------

	public:
		std::size_t Offset() const;

		LayoutBuffer& Offset(std::size_t const offset);

	private:
		std::size_t mOffset{};

	// --------------------------------

	public:
		std::size_t Size() const;

		LayoutBuffer& Size(std::size_t const size);

	private:
		std::size_t mSize{};
	};
}
