#pragma once
#include <pch.h>

#include <memory>
#include <type_traits>
#include <boost/core/noncopyable.hpp>
#include <aiva/layer1/i_graphic_command_async.h>
#include <aiva/layer1/i_graphic_command_sync.h>

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
		GraphicExecutor(aiva::layer1::Engine const& engine);

		~GraphicExecutor();

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// Commands

	public:
		template <typename T, typename std::enable_if_t<std::is_base_of_v<aiva::layer1::IGraphicCommandAsync, T>, bool> = true>
		void ExecuteCommand(T const& command);

		template <typename T, typename std::enable_if_t<std::is_base_of_v<aiva::layer1::IGraphicCommandSync, T>, bool> = true>
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

template <typename T, typename std::enable_if_t<std::is_base_of_v<aiva::layer1::IGraphicCommandAsync, T>, bool>>
void aiva::layer1::GraphicExecutor::ExecuteCommand(T const& command)
{
	mPendingCommands.emplace_back(std::make_unique<T>(command));
}

template <typename T, typename std::enable_if_t<std::is_base_of_v<aiva::layer1::IGraphicCommandSync, T>, bool>>
void aiva::layer1::GraphicExecutor::ExecuteCommand(T const& command)
{
	command.Execute(mEngine);
}
