#pragma once
#include <pch.h>

#include <aiva/utils_ext/m_single_arg.h>
#include <aiva/utils_ext/t_object.h>

#define M_OBJECT_BODY(ObjectType)																						\
/* ----------------------------------------------------------------------------------------------------------------- */	\
																														\
public:																													\
	using ThisType = M_SINGLE_ARG(ObjectType);																			\
																														\
	friend aiva::utils_ext::TObject<M_SINGLE_ARG(ObjectType)>;															\
																														\
/* ----------------------------------------------------------------------------------------------------------------- */	\
