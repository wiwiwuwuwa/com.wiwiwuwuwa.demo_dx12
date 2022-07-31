#pragma once
#include <pch.h>

#include <aiva/layer1/i_cpu_resource.h>

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
	struct RoSceneGlb final : public aiva::layer1::ICpuResource
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<aiva::layer1::RoSceneGlb> Create(Args&&... args);

	private:
		RoSceneGlb(aiva::layer1::Engine const& engine);

	public:
		~RoSceneGlb();

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// ICpuResource

	public:
		void DeserealizeFromBinary(std::vector<std::byte> const& binaryData) override;

	// ----------------------------------------------------
	// Cache Refresh

	public:
		using CacheUpdaterType = aiva::utils::TCacheUpdater<RoSceneGlb>;

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
std::shared_ptr<aiva::layer1::RoSceneGlb> aiva::layer1::RoSceneGlb::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer1::RoSceneGlb>{new aiva::layer1::RoSceneGlb{ std::forward<Args>(args)... }};
}
