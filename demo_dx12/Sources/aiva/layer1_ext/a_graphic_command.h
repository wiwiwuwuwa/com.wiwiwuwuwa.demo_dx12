#pragma once
#include <pch.h>

#include <aiva/layer1_ext/a_graphic_command_fwd.h>
#include <aiva/utils_ext/m_object_field_val_event_action.h>

namespace aiva::layer1_ext
{
	struct AGraphicCommand
	{
	// ----------------------------------------------------
	// Main

	public:
		using ThisType = AGraphicCommand;

	protected:
		AGraphicCommand();

	public:
		virtual ~AGraphicCommand();

	// ----------------------------------------------------
	// Fields

	public:
		void Init();

		void Exec();

		void Shut();

	public:
		M_OBJECT_FIELD_VAL_EVENT_ACTION_A2(public, OnAfterInit);

		M_OBJECT_FIELD_VAL_EVENT_ACTION_A2(public, OnBeforeShut);

	protected:
		M_OBJECT_FIELD_VAL_EVENT_ACTION_A2(protected, OnInit);

		M_OBJECT_FIELD_VAL_EVENT_ACTION_A2(protected, OnExec);

		M_OBJECT_FIELD_VAL_EVENT_ACTION_A2(protected, OnShut);
	};
}
