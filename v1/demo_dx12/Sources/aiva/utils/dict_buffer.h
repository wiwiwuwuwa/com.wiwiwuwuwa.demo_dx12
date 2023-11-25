#pragma once
#include <pch.h>

#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_changeable.h>

namespace aiva::utils
{
	struct DictStruct;
	struct LayoutStruct;
	struct MetaStruct;
}

namespace aiva::utils
{
	struct DictBuffer final : public AObject, public IObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		DictBuffer();

	public:
		~DictBuffer() override;

	// ----------------------------------------------------
	// Aliases

	public:
		using DictStructElementType = DictStruct;

		using DictStructPointerType = std::shared_ptr<DictStructElementType>;

		using DictStructPointerArrayType = std::vector<DictStructPointerType>;

		using LayoutStructElementType = LayoutStruct;

		using LayoutStructPointerType = std::shared_ptr<LayoutStruct>;

		using MetaStructElementType = MetaStruct;

		using MetaStructPointerType = std::shared_ptr<MetaStruct>;

	// ----------------------------------------------------
	// Layout

	public:
		MetaStructPointerType const& Layout() const;

		DictBuffer& Layout(DictStructPointerType const& layout);

		DictBuffer& Layout(LayoutStructPointerType const& layout);

		DictBuffer& Layout(MetaStructPointerType const& layout);

	private:
		void Layout_OnChanged();

	private:
		MetaStructPointerType mLayout{};

	// ----------------------------------------------------
	// Structs

	public:
		DictBuffer& Add(DictStructPointerType const& dictStruct);

		DictStructPointerType const& Get(std::size_t index) const;

		std::size_t Num() const;

	private:
		void Struct_OnChanged();

	private:
		DictStructPointerArrayType mStructs{};
	};
}
