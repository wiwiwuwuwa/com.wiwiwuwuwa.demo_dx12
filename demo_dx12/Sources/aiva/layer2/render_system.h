#pragma once
#include <pch.h>

namespace aiva::layer2
{
	struct World;
}

namespace aiva::layer2
{
	struct RenderSystem final : private boost::noncopyable, public std::enable_shared_from_this<RenderSystem>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<RenderSystem> Create(Args&&... args);

	private:
		RenderSystem(World const& world);

	public:
		~RenderSystem();

	private:
		World const& mWorld;
	};
}

// --------------------------------------------------------

template <typename... Args>
std::shared_ptr<aiva::layer2::RenderSystem> aiva::layer2::RenderSystem::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer2::RenderSystem>(new aiva::layer2::RenderSystem{ std::forward<Args>(args)... });
}
