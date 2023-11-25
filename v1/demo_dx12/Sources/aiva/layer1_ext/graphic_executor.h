#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/a_graphic_command_fwd.h>
#include <aiva/layer1_ext/graphic_executor_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct GraphicExecutor final : aiva::utils_ext::TObject<GraphicExecutor>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(GraphicExecutor);

	protected:
		GraphicExecutor(aiva::layer1::Engine& engine);

	public:
		~GraphicExecutor() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

	// ----------------------------------------------------
	// Commands Fields

	public:
		M_OBJECT_FIELD_VAR_3(private, std::deque<std::shared_ptr<AGraphicCommand>>, PendingCommands);

		M_OBJECT_FIELD_VAR_3(private, std::deque<std::shared_ptr<AGraphicCommand>>, ExecutingCommands);

	public:
		void Execute(std::shared_ptr<AGraphicCommand> const& command);

	private:
		void InitCommandsFields();

		void ShutCommandsFields();

	private:
		void CommandsFields_When_GraphicPipeline_OnExecuteCommands();

		void CommandsFields_When_GraphicPipeline_OnCleanupCommands();
	};
}
