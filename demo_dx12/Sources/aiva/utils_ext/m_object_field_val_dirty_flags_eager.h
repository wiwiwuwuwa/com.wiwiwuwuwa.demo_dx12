#pragma once
#include <pch.h>

#include <aiva/utils_ext/m_object_field_val.h>
#include <aiva/utils_ext/m_single_arg.h>
#include <aiva/utils_ext/t_dirty_flags_eager.h>

#define M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER_2(FieldName, FieldAccess) M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER_3(FieldName, FieldAccess, M_SINGLE_ARG(aiva::utils_ext::EDirtyFlags))
#define M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER_3(FieldName, FieldAccess, FlagsType) M_OBJECT_FIELD_VAL_4(M_SINGLE_ARG(aiva::utils_ext::TDirtyFlagsEager<ThisType, M_SINGLE_ARG(FlagsType)>), FieldName, FieldAccess, FieldAccess)
