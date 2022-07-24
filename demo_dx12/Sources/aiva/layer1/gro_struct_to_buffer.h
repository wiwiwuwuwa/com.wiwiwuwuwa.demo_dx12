#pragma once
#include <pch.h>

#include <aiva/layer1/gro_struct_to_buffer_desc.h>
#include <aiva/layer1/i_gpu_resource_operator.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	struct Engine;
	struct ShaderStruct;
}

namespace aiva::layer1
{
	struct GroStructToBuffer final : private boost::noncopyable, public std::enable_shared_from_this<GroStructToBuffer>, public IGpuResourceOperator
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<GroStructToBuffer> Create(TArgs&&... args);

	private:
		GroStructToBuffer(Engine const& engine, GroStructToBufferDesc const& desc);

	public:
		~GroStructToBuffer();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Desc Data

	public:
		GroStructToBufferDesc const& Desc() const;

		GroStructToBuffer& Desc(GroStructToBufferDesc const& desc);

	private:
		GroStructToBufferDesc mDesc{};

	// ----------------------------------------------------
	// Struct Data

	public:
		ShaderStruct& Struct() const;

		GroStructToBuffer& ApplyChanges();

	private:
		void InitializeStruct();

		void TerminateStruct();

	private:
		std::shared_ptr<ShaderStruct> mStruct{};

	// ----------------------------------------------------
	// Updated Event

	public:
		aiva::utils::EvAction& OnInternalResourceUpdated();

	private:
		void RefreshInternalResourceUpdated(GroStructToBufferDesc const& previousDesc, GroStructToBufferDesc const& desiredDesc);

		void NotifyInternalResourceUpdated();

	private:
		aiva::utils::EvAction mOnInternalResourceUpdated{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GroStructToBuffer> aiva::layer1::GroStructToBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<GroStructToBuffer>{new GroStructToBuffer{ std::forward<TArgs>(args)... }};
}
