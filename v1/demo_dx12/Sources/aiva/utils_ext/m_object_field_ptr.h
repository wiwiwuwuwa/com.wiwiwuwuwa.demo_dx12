#pragma once
#include <pch.h>

#include <aiva/utils_ext/m_object_field_val.h>
#include <aiva/utils_ext/m_single_arg.h>

#define M_OBJECT_FIELD_PTR_4(FieldType, FieldName, GetterAccess, SetterAccess) M_OBJECT_FIELD_VAL_4(M_SINGLE_ARG(FieldType*), FieldName, GetterAccess, SetterAccess)
#define M_OBJECT_FIELD_PTR_5(FieldType, FieldName, FieldValue, GetterAccess, SetterAccess) M_OBJECT_FIELD_VAL_5(M_SINGLE_ARG(FieldType*), FieldName, M_SINGLE_ARG(FieldValue), GetterAccess, SetterAccess)
