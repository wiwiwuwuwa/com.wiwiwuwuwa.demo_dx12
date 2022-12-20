#pragma once
#include <pch.h>

#include <aiva2/engine/m_field_by_val.h>
#include <aiva2/engine/m_single_arg.h>
#include <aiva2/engine/t_event_action.h>

#define M_FIELD_FOR_EVENT_2(FieldAccess, FieldName) M_FIELD_FOR_EVENT_3(FieldAccess, FieldName, ThisType)
#define M_FIELD_FOR_EVENT_3(FieldAccess, FieldName, FieldOwner) M_FIELD_BY_VAL_4(FieldAccess, FieldAccess, M_SINGLE_ARG(aiva2::engine::TEventActionReadWrite<M_SINGLE_ARG(FieldOwner)>), FieldName)
