#pragma once
#include <pch.h>

#include <aiva/utils_ext/m_single_arg.h>

#define M_OBJECT_FIELD_VAL_4(FieldType, FieldName, GetterAccess, SetterAccess) M_OBJECT_FIELD_VAL_5(M_SINGLE_ARG(FieldType), FieldName, M_SINGLE_ARG(), GetterAccess, SetterAccess)
#define M_OBJECT_FIELD_VAL_5(FieldType, FieldName, FieldValue, GetterAccess, SetterAccess)																							\
/* ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */	\
																																													\
GetterAccess:																																										\
	FieldType const& FieldName() const																																				\
	{																																												\
		return m##FieldName;																																						\
	}																																												\
																																													\
SetterAccess:																																										\
	FieldType& FieldName()																																							\
	{																																												\
		return m##FieldName;																																						\
	}																																												\
																																													\
private:																																											\
	FieldType m##FieldName{ FieldValue };																																			\
																																													\
/* ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */	\
