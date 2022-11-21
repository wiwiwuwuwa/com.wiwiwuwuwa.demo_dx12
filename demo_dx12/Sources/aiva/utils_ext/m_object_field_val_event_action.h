#pragma once
#include <pch.h>

#include <aiva/utils_ext/m_object_field_val.h>
#include <aiva/utils_ext/m_single_arg.h>
#include <aiva/utils_ext/t_event_action.h>

#define M_OBJECT_FIELD_VAL_EVENT_ACTION_A2(FieldAccess, FieldName) M_OBJECT_FIELD_VAL_EVENT_ACTION_A3(FieldAccess, M_SINGLE_ARG(ThisType), FieldName)
#define M_OBJECT_FIELD_VAL_EVENT_ACTION_A3(FieldAccess, FieldOwner, FieldName) M_OBJECT_FIELD_VAL_4(M_SINGLE_ARG(aiva::utils_ext::TEventAction<M_SINGLE_ARG(FieldOwner)>), FieldName, FieldAccess, FieldAccess)

#define M_OBJECT_FIELD_VAL_EVENT_ACTION_B2(FieldAccess, FieldName, ...) M_OBJECT_FIELD_VAL_EVENT_ACTION_B3(FieldAccess, M_SINGLE_ARG(ThisType), FieldName, __VA_ARGS__)
#define M_OBJECT_FIELD_VAL_EVENT_ACTION_B3(FieldAccess, FieldOwner, FieldName, ...) M_OBJECT_FIELD_VAL_4(M_SINGLE_ARG(aiva::utils_ext::TEventAction<M_SINGLE_ARG(FieldOwner), __VA_ARGS__>), FieldName, FieldAccess, FieldAccess)
