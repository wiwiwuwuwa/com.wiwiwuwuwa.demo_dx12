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
	mPendingCommands = {};
	mEngine.GraphicPipeline().OnPopulateCommands().connect(boost::bind(&aiva::layer1::GraphicExecutor::TickCommands, this));
}

void aiva::layer1::GraphicExecutor::TickCommands()
{
	while (!mPendingCommands.empty())
	{
		std::unique_ptr<aiva::layer1::IGraphicCommandAsync> const& command = mPendingCommands.front();
		aiva::utils::Asserts::CheckBool(command);

		command->Execute(mEngine);
		mPendingCommands.pop();
	}
}

void aiva::layer1::GraphicExecutor::TerminateCommands()
{
	mEngine.GraphicPipeline().OnPopulateCommands().disconnect(boost::bind(&aiva::layer1::GraphicExecutor::TickCommands, this));
	mPendingCommands = {};
}
