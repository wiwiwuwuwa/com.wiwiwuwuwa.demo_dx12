#pragma once
#include <pch.h>

#include <aiva/utils_ext/m_single_arg.h>

#define M_OBJECT_FIELD_VAR_CUSTOM_4(FieldType, FieldName, GetterAccess, SetterAccess) M_OBJECT_FIELD_VAR_CUSTOM_5(M_SINGLE_ARG(FieldType), FieldName, M_SINGLE_ARG(), GetterAccess, SetterAccess)
#define M_OBJECT_FIELD_VAR_CUSTOM_5(FieldType, FieldName, FieldValue, GetterAccess, SetterAccess)																					\
/* ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */	\
																																													\
GetterAccess:																																										\
	FieldType const& FieldName();																																					\
																																													\
SetterAccess:																																										\
	void FieldName(FieldType const& value);																																			\
																																													\
private:																																											\
	FieldType m##FieldName{ FieldValue };																																			\
																																													\
/* ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */	\
