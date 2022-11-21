#include <pch.h>
#include <aiva/layer1_ext/a_graphic_command.h>

namespace aiva::layer1_ext
{
	AGraphicCommand::AGraphicCommand()
	{

	}

	AGraphicCommand::~AGraphicCommand()
	{

	}

	void AGraphicCommand::Init()
	{
		OnInit().Broadcast();
		OnAfterInit().Broadcast();
	}

	void AGraphicCommand::Exec()
	{
		OnExec().Broadcast();
	}

	void AGraphicCommand::Shut()
	{
		OnBeforeShut().Broadcast();
		OnShut().Broadcast();
	}
}
