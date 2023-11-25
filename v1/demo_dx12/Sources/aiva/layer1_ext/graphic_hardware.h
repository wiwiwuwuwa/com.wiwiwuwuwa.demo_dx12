#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/graphic_hardware_fwd.h>
#include <aiva/layer1_ext/ro_texture_2d_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct GraphicHardware final : public aiva::utils_ext::TObject<GraphicHardware>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(GraphicHardware);

	protected:
		GraphicHardware(aiva::layer1::Engine& engine);

	public:
		~GraphicHardware() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

	// ----------------------------------------------------
	// Graphic Hardware Shortcuts

	public:
		ID3D12GraphicsCommandList6& CommandList() const;

		ID3D12Device9& Device() const;

	// ----------------------------------------------------
	// Screen Params Fields

	public:
		M_OBJECT_FIELD_VAR_3(private, std::vector<std::shared_ptr<RoTexture2D>>, ScreenTextures);

	public:
		std::shared_ptr<RoTexture2D> const& ScreenTexture() const;

		glm::size2 ScreenSize() const;

	private:
		void InitScreenParamsFields();

		void ShutScreenParamsFields();
	};
}
