#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource.h>
#include <aiva/layer1/e_resource_memory_type.h>

namespace aiva::layer1
{
	struct Engine;
	struct GraphicResourceFactory;
}

namespace aiva::layer1
{
	struct GrBuffer final : public AGraphicResource
	{
	// ----------------------------------------------------
	// Main

	private:
		friend GraphicResourceFactory;

	private:
		GrBuffer(aiva::layer1::Engine const& engine);

	// ----------------------------------------------------
	// Metadata

	public:
		EResourceMemoryType MemoryType() const;

		GrBuffer& MemoryType(EResourceMemoryType const memoryType);

	private:
		EResourceMemoryType mMemoryType{};

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
		void RefreshInternalResource(winrt::com_ptr<ID3D12Resource>& resource, aiva::utils::ResourceBarrier& barrier) override;
	};
}
