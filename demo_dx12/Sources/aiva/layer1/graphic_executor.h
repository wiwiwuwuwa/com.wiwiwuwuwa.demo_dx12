#pragma once
#include <pch.h>

#include <memory>
#include <type_traits>
#include <boost/core/noncopyable.hpp>
#include <aiva/layer1/i_graphic_command_async.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct GraphicExecutor final : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Main

	public:
		GraphicExecutor(Engine const& engine);

		~GraphicExecutor();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Commands

	public:
		template <typename T>
		void ExecuteCommand(T const& command);

	private:
		void InitializeCommands();

		void CleanupCommands();

		void ExecuteCommands();

		void TerminateCommands();

	private:
		std::list<std::unique_ptr<IGraphicCommandAsync>> mPendingCommands{};
	};
}

// --------------------------------------------------------

template <typename T>
void aiva::layer1::GraphicExecutor::ExecuteCommand(T const& command)
{
	mPendingCommands.emplace_back(std::make_unique<T>(command));
}
