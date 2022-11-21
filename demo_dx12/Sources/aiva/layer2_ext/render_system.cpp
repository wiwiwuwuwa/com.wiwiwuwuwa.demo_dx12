#include <pch.h>
#include <aiva/layer2_ext/render_system.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_pipeline.h>
#include <aiva/layer1_ext/gc_clear_render_target.h>
#include <aiva/layer1_ext/gc_draw_mesh.h>
#include <aiva/layer1_ext/gc_set_render_target.h>
#include <aiva/layer1_ext/gc_set_scissor_rects.h>
#include <aiva/layer1_ext/gc_set_viewports.h>
#include <aiva/layer1_ext/gc_present.h>
#include <aiva/layer1_ext/graphic_executor.h>
#include <aiva/layer1_ext/graphic_hardware.h>
#include <aiva/layer1_ext/nr_texture_2d.h>
#include <aiva/layer1_ext/resource_system.h>
#include <aiva/layer1_ext/ro_material_gr.h>
#include <aiva/layer1_ext/ro_mesh.h>
#include <aiva/layer1_ext/ro_texture_2d.h>
#include <aiva/layer1_ext/rt_render_identity.h>
#include <aiva/layer1_ext/rt_render_identity_multiple.h>
#include <aiva/layer2/world.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer2_ext
{
	using namespace aiva::layer1_ext;
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	RenderSystem::RenderSystem(aiva::layer2::World& world) : mWorld{ world }
	{
		InitRendererFields();
		InitFieldsForTesting();
	}

	RenderSystem::~RenderSystem()
	{
		ShutRendererFields();
		ShutFieldsForTesting();
	}

	void RenderSystem::InitRendererFields()
	{
		mWorld.Engine().GraphicPipeline().OnPopulateCommands().connect(boost::bind(&ThisType::RendererFields_When_GraphicPipeline_OnPopulateCommands, this));
	}

	void RenderSystem::ShutRendererFields()
	{
		mWorld.Engine().GraphicPipeline().OnPopulateCommands().disconnect(boost::bind(&ThisType::RendererFields_When_GraphicPipeline_OnPopulateCommands, this));
	}

	void RenderSystem::RendererFields_When_GraphicPipeline_OnPopulateCommands()
	{
		{
			auto const command = NewObject<GcSetScissorRects>(mWorld.Engine());
			command->Rect({ 0.0f, 0.0f, mWorld.Engine().GraphicHardwareExt().ScreenSize() });
			mWorld.Engine().GraphicExecutorExt().Execute(command);
		}

		{
			auto const command = NewObject<GcSetViewports>(mWorld.Engine());
			command->Rect({ 0.0f, 0.0f, mWorld.Engine().GraphicHardwareExt().ScreenSize() });
			mWorld.Engine().GraphicExecutorExt().Execute(command);
		}

		{
			auto const command = NewObject<GcSetRenderTarget>(mWorld.Engine());
			command->RenderTargets()->InsertResource(mWorld.Engine().GraphicHardwareExt().ScreenTexture(), {}, {});
			mWorld.Engine().GraphicExecutorExt().Execute(command);
		}

		{
			auto const command = NewObject<GcClearRenderTarget>(mWorld.Engine());
			command->Target()->AttachResource(mWorld.Engine().GraphicHardwareExt().ScreenTexture(), {});
			command->Color({ 1.0f, 0.5f, 0.0f, 1.0f });
			mWorld.Engine().GraphicExecutorExt().Execute(command);
		}

		{
			auto const command = NewObject<GcDrawMesh>(mWorld.Engine());
			command->Geometry(mTestGeometry);
			command->Material(mTestMaterial);
			mWorld.Engine().GraphicExecutorExt().Execute(command);
		}

		{
			auto const command = NewObject<GcPresent>(mWorld.Engine());
			mWorld.Engine().GraphicExecutorExt().Execute(command);
		}
	}

	void RenderSystem::InitFieldsForTesting()
	{
		mTestGeometry = mWorld.Engine().ResourceSystemExt().GetResource<RoMesh>("resources\\meshes\\cube.json");
		mTestMaterial = mWorld.Engine().ResourceSystemExt().GetResource<RoMaterialGr>("resources\\materials\\cube.json");
	}

	void RenderSystem::ShutFieldsForTesting()
	{
		mTestMaterial = {};
		mTestGeometry = {};
	}
}
