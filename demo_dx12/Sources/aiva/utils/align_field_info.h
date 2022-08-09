#pragma once
#include <pch.h>

#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_changeable.h>

namespace aiva::utils
{
	struct AlignFieldInfo final : public AObject, public IObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		AlignFieldInfo();

	public:
		~AlignFieldInfo() override;

	// ----------------------------------------------------
	// Data

	public:
		std::size_t Offset() const;

		AlignFieldInfo& Offset(std::size_t const offset);

	private:
		std::size_t mOffset{};
	};
}
