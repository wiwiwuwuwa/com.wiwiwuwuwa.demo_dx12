#pragma once
#include <pch.h>

#include <aiva/utils_ext/m_object_field_val.h>
#include <aiva/utils_ext/m_single_arg.h>
#include <aiva/utils_ext/t_dirty_flags_lazy.h>

#define M_OBJECT_FIELD_VAL_DIRTY_FLAGS_LAZY() M_OBJECT_FIELD_VAL_DIRTY_FLAGS_LAZY_2(M_SINGLE_ARG(DirtyFlags), M_SINGLE_ARG(public))
#define M_OBJECT_FIELD_VAL_DIRTY_FLAGS_LAZY_2(FieldName, FieldAccess) M_OBJECT_FIELD_VAL_DIRTY_FLAGS_LAZY_3(M_SINGLE_ARG(FieldName), M_SINGLE_ARG(Nothing), M_SINGLE_ARG(FieldAccess))
#define M_OBJECT_FIELD_VAL_DIRTY_FLAGS_LAZY_3(FieldName, FieldValue, FieldAccess) M_OBJECT_FIELD_VAL_DIRTY_FLAGS_LAZY_4(M_SINGLE_ARG(FieldName), M_SINGLE_ARG(FieldValue), M_SINGLE_ARG(FieldAccess), M_SINGLE_ARG(aiva::utils_ext::EDirtyFlags))
#define M_OBJECT_FIELD_VAL_DIRTY_FLAGS_LAZY_4(FieldName, FieldValue, FieldAccess, FlagsType) M_OBJECT_FIELD_VAL_5(M_SINGLE_ARG(aiva::utils_ext::TDirtyFlagsLazy<ThisType, M_SINGLE_ARG(FlagsType)>), FieldName, M_SINGLE_ARG(FlagsType##::##FieldValue), M_SINGLE_ARG(FieldAccess), M_SINGLE_ARG(FieldAccess))
