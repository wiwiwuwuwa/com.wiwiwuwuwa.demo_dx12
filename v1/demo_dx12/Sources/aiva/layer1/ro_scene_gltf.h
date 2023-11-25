#pragma once
#include <pch.h>

#include <aiva/layer1/i_cpu_resource.h>
#include <aiva/layer1/ro_scene_gltf_fwd.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::utils
{
	enum class ECacheFlags : std::uint8_t;

	template <typename, typename = ECacheFlags>
	struct TCacheUpdater;
}

namespace aiva::layer1
{
	struct RoSceneGltf final : public aiva::layer1::ICpuResource
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<aiva::layer1::RoSceneGltf> Create(Args&&... args);

		Engine const& Engine() const;

	private:
		RoSceneGltf(aiva::layer1::Engine const& engine);

	public:
		~RoSceneGltf();

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// ICpuResource

	public:
		void DeserealizeFromBinary(std::vector<std::byte> const& binaryData) override;

	// ----------------------------------------------------
	// Cache Refresh

	public:
		using CacheUpdaterType = aiva::utils::TCacheUpdater<RoSceneGltf>;

	public:
		CacheUpdaterType& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};

	// ----------------------------------------------------
	// Resource

	public:
		tinygltf::Model const& Model() const;

	private:
		tinygltf::Model mModel{};
	};
}

// --------------------------------------------------------

template <typename... Args>
std::shared_ptr<aiva::layer1::RoSceneGltf> aiva::layer1::RoSceneGltf::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer1::RoSceneGltf>{new aiva::layer1::RoSceneGltf{ std::forward<Args>(args)... }};
}
