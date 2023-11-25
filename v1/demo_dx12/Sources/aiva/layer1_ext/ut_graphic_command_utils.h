#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/nr_buffer_fwd.h>
#include <aiva/layer1_ext/nr_texture_2d_fwd.h>
#include <aiva/layer1_ext/ro_mesh_gltf_fwd.h>

namespace aiva::layer1_ext
{
	struct UtGraphicCommandUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		UtGraphicCommandUtils() = delete;

	// ----------------------------------------------------
	// Generic Fields

	public:
		static void CopyDataToBuffer(boost::span<const std::byte> const& srcData, std::shared_ptr<NrBuffer> const& dstBuffer, aiva::layer1::Engine& engine);

		static void CopyBufferToBuffer(std::shared_ptr<NrBuffer> const& srcBuffer, std::shared_ptr<NrBuffer> const& dstBuffer, aiva::layer1::Engine& engine);

		static void CopyDataToTexture2D(boost::span<const std::byte> const& srcBin, std::shared_ptr<NrTexture2D> const& dstObj, aiva::layer1::Engine& engine);

		static void CopyBufferToTexture2D(std::shared_ptr<NrBuffer> const& srcObj, std::shared_ptr<NrTexture2D> const& dstObj, aiva::layer1::Engine& engine);
	};
}
