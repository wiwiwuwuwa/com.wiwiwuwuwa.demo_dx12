#pragma once
#include <pch.h>

#include <aiva2/engine/m_single_arg.h>

#define M_FIELD_BY_VAR_3(FieldAccess, FieldType, FieldName) M_FIELD_BY_VAR_4(FieldAccess, FieldAccess, M_SINGLE_ARG(FieldType), FieldName)
#define M_FIELD_BY_VAR_4(GetterAccess, SetterAccess, FieldType, FieldName) M_FIELD_BY_VAR_5(GetterAccess, SetterAccess, M_SINGLE_ARG(FieldType), FieldName, M_SINGLE_ARG())
#define M_FIELD_BY_VAR_5(GetterAccess, SetterAccess, FieldType, FieldName, FieldValue)                                                                                            \
/* --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */ \
                                                                                                                                                                                  \
GetterAccess:                                                                                                                                                                     \
	FieldType const& FieldName() const                                                                                                                                            \
	{                                                                                                                                                                             \
		return m##FieldName;                                                                                                                                                      \
	}                                                                                                                                                                             \
                                                                                                                                                                                  \
SetterAccess:                                                                                                                                                                     \
	void FieldName(FieldType const& value)                                                                                                                                        \
	{                                                                                                                                                                             \
		m##FieldName = value;                                                                                                                                                     \
	}                                                                                                                                                                             \
                                                                                                                                                                                  \
private:                                                                                                                                                                          \
	FieldType m##FieldName{ FieldValue };                                                                                                                                         \
                                                                                                                                                                                  \
/* --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */ \
