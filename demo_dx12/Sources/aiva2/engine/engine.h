#pragma once
#include <pch.h>

#include <aiva2/engine/engine_fwd.h>
#include <aiva2/engine/i_object.h>
#include <aiva2/engine/m_struct_body.h>
#include <aiva2/native/engine_fwd.h>

namespace aiva2::engine
{
	struct Engine : public IObject
	{
	// ----------------------------------------------------
	// Main

	public:
		M_STRUCT_BODY(Engine);

	public:
		Engine();

		~Engine() override;

	public:
		void Run() const;

	// ----------------------------------------------------
	// Native

	public:
		auto Native() const -> native::Engine&;

	private:
		void InitNative();

		void ShutNative();

	private:
		std::shared_ptr<native::Engine> mNative{};
	};
}
