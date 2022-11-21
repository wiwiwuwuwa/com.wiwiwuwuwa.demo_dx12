#include <pch.h>
#include <aiva/layer1_ext/graphic_executor.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_pipeline.h>
#include <aiva/layer1_ext/a_graphic_command.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	GraphicExecutor::GraphicExecutor(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitCommandsFields();
	}

	GraphicExecutor::~GraphicExecutor()
	{
		ShutCommandsFields();
	}

	void GraphicExecutor::Execute(std::shared_ptr<AGraphicCommand> const& command)
	{
		Asserts::CheckBool(command, "Command is not valid");
		
		command->Init();
		mPendingCommands.push_back(command);
	}

	void GraphicExecutor::InitCommandsFields()
	{
		Engine().GraphicPipeline().OnExecuteCommands().connect(boost::bind(&ThisType::CommandsFields_When_GraphicPipeline_OnExecuteCommands, this));
		Engine().GraphicPipeline().OnCleanupCommands().connect(boost::bind(&ThisType::CommandsFields_When_GraphicPipeline_OnCleanupCommands, this));
	}

	void GraphicExecutor::ShutCommandsFields()
	{
		Engine().GraphicPipeline().OnCleanupCommands().disconnect(boost::bind(&ThisType::CommandsFields_When_GraphicPipeline_OnCleanupCommands, this));
		Engine().GraphicPipeline().OnExecuteCommands().disconnect(boost::bind(&ThisType::CommandsFields_When_GraphicPipeline_OnExecuteCommands, this));
	}

	void GraphicExecutor::CommandsFields_When_GraphicPipeline_OnExecuteCommands()
	{
		while (!std::empty(mPendingCommands))
		{
			auto const& command = mPendingCommands.front();
			Asserts::CheckBool(command, "Command is not valid");

			command->Exec();

			mExecutingCommands.push_back(command);
			mPendingCommands.pop_front();
		}
	}

	void GraphicExecutor::CommandsFields_When_GraphicPipeline_OnCleanupCommands()
	{
		while (!std::empty(mExecutingCommands))
		{
			auto const& command = mExecutingCommands.front();
			Asserts::CheckBool(command, "Command is not valid");

			command->Shut();
			mExecutingCommands.pop_front();
		}
	}
}
