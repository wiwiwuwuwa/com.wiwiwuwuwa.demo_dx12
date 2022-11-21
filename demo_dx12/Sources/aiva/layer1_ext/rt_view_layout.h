#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_view_layout_type.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtViewLayoutType>
	struct RtViewLayout;

	// ----------------------------------------------------
	// Specializations

	template <>
	struct RtViewLayout<RtViewLayoutType::CPU>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_FIELD_VAR_3(public, std::size_t, Index);

	public:
		bool operator<(RtViewLayout const& other) const
		{
			if (mIndex != other.mIndex)
			{
				return mIndex < other.mIndex;
			}

			return false;
		}
	};

	template <>
	struct RtViewLayout<RtViewLayoutType::GPU>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_FIELD_VAR_3(public, std::size_t, Index);

		M_OBJECT_FIELD_VAR_3(public, std::size_t, Space);

	public:
		bool operator<(RtViewLayout const& other) const
		{
			if (mIndex != other.mIndex)
			{
				return mIndex < other.mIndex;
			}

			if (mSpace != other.mSpace)
			{
				return mSpace < other.mSpace;
			}

			return false;
		}
	};
}
