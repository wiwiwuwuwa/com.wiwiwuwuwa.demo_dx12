#include <pch.h>
#include <aiva/layer1/graphic_executor.h>

#include <utility>
#include <boost/bind.hpp>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_pipeline.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GraphicExecutor::GraphicExecutor(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeCommands();
}

aiva::layer1::GraphicExecutor::~GraphicExecutor()
{
	TerminateCommands();
}

void aiva::layer1::GraphicExecutor::InitializeCommands()
{
	mPendingCommands.clear();
	mEngine.GraphicPipeline().OnCleanupCommands().connect(boost::bind(&aiva::layer1::GraphicExecutor::CleanupCommands, this));
	mEngine.GraphicPipeline().OnExecuteCommands().connect(boost::bind(&aiva::layer1::GraphicExecutor::ExecuteCommands, this));
}

void aiva::layer1::GraphicExecutor::CleanupCommands()
{
	mPendingCommands.clear();
}

void aiva::layer1::GraphicExecutor::ExecuteCommands()
{
	for (auto const& command : mPendingCommands)
	{
		aiva::utils::Asserts::CheckBool(command);
		command->Execute(mEngine);
	}
}

void aiva::layer1::GraphicExecutor::TerminateCommands()
{
	mEngine.GraphicPipeline().OnExecuteCommands().disconnect(boost::bind(&aiva::layer1::GraphicExecutor::ExecuteCommands, this));
	mEngine.GraphicPipeline().OnCleanupCommands().disconnect(boost::bind(&aiva::layer1::GraphicExecutor::CleanupCommands, this));
	mPendingCommands.clear();
}
