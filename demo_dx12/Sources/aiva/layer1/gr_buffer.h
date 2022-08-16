#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource.h>
#include <aiva/layer1/e_buffer_memory_type.h>
#include <aiva/layer1/gr_buffer_fwd.h>

namespace aiva::layer1
{
	struct GrBuffer final : public AGraphicResource
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		GrBuffer(EngineType const& engine);

	public:
		~GrBuffer() override;

	// ----------------------------------------------------
	// Aliases

	public:
		using MemoryTypeEnum = EBufferMemoryType;

	// ----------------------------------------------------
	// Metadata

	public:
		MemoryTypeEnum MemoryType() const;

		GrBuffer& MemoryType(MemoryTypeEnum const memoryType);

	private:
		MemoryTypeEnum mMemoryType{ MemoryTypeEnum::CpuToGpu };

	// --------------------------------

	public:
		std::size_t Size() const;

		GrBuffer& Size(std::size_t const size);

	private:
		std::size_t mSize{};

	// --------------------------------

	public:
		bool SupportShaderAtomics() const;

		GrBuffer& SupportShaderAtomics(bool const support);

	private:
		bool mSupportShaderAtomics{};

	// --------------------------------

	public:
		bool SupportUnorderedAccess() const;

		GrBuffer& SupportUnorderedAccess(bool const support);

	private:
		bool mSupportUnorderedAccess{};

	// ----------------------------------------------------
	// Internal Resource

	protected:
		void RefreshInternalResourceFromSelf(ResourceType& resource, BarrierType& barrier) override;

		void RefreshSelfFromInternalResource(ResourceType const& resource) override;
	};
}
