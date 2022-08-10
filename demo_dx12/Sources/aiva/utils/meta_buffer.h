#pragma once
#include <pch.h>

#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_changeable.h>

namespace aiva::utils
{
	struct MetaStruct;
}

namespace aiva::utils
{
	struct MetaBuffer final : public AObject, public IObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		MetaBuffer();

	public:
		~MetaBuffer() override;

	// ----------------------------------------------------
	// Metadata

	public:
		using MetaStructElementType = MetaStruct;

		using MetaStructPointerType = std::shared_ptr<MetaStructElementType>;

	public:
		MetaStructPointerType const& Layout() const;

		MetaBuffer& Layout(MetaStructPointerType const& layout);

	private:
		void Layout_OnChanged();

	private:
		MetaStructPointerType mLayout{};

	// ----------------------------------------------------

	public:
		std::size_t Count() const;

		MetaBuffer& Count(std::size_t const count);

	private:
		std::size_t mCount{};
	};
}