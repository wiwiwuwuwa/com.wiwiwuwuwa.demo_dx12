#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource.h>
#include <aiva/layer1/e_resource_buffer_format.h>
#include <aiva/layer1/gr_texture_2d_fwd.h>

namespace aiva::layer1
{
	struct GrTexture2D final : public AGraphicResource
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		GrTexture2D(EngineType const& engine);

	public:
		~GrTexture2D() override;

	// ----------------------------------------------------
	// Metadata

	public:
		EResourceBufferFormat Format() const;

		GrTexture2D& Format(EResourceBufferFormat const format);

	private:
		EResourceBufferFormat mFormat{ EResourceBufferFormat::R32G32B32A32_FLOAT };

	// --------------------------------

	public:
		std::size_t Width() const;

		GrTexture2D& Width(std::size_t const width);

	private:
		std::size_t mWidth{};

	// --------------------------------

	public:
		std::size_t Height() const;

		GrTexture2D& Height(std::size_t const height);

	private:
		std::size_t mHeight{};

	// --------------------------------

	public:
		bool SupportDepthStencil() const;

		GrTexture2D& SupportDepthStencil(bool const support);

	private:
		bool mSupportDepthStencil{};

	// --------------------------------

	public:
		bool SupportMipMapping() const;

		GrTexture2D& SupportMipMapping(bool const support);

	private:
		bool mSupportMipMapping{};

	// --------------------------------

	public:
		bool SupportRenderTarget() const;

		GrTexture2D& SupportRenderTarget(bool const support);

	private:
		bool mSupportRenderTarget{};

	// --------------------------------

	public:
		bool SupportShaderAtomics() const;

		GrTexture2D& SupportShaderAtomics(bool const support);

	private:
		bool mSupportShaderAtomics{};

	// --------------------------------

	public:
		bool SupportUnorderedAccess() const;

		GrTexture2D& SupportUnorderedAccess(bool const support);

	private:
		bool mSupportUnorderedAccess{};

	// ----------------------------------------------------
	// Internal Resource

	protected:
		void RefreshInternalResourceFromSelf(ResourceType& resource, BarrierType& barrier) override;

		void RefreshSelfFromInternalResource(ResourceType const& resource) override;
	};
}
