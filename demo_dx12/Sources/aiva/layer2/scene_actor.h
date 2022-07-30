#pragma once
#include <pch.h>

namespace aiva::layer2
{
	struct SceneComponent;
	struct SceneSystem;
	struct World;
}

namespace aiva::layer2
{
	struct SceneActor final : private boost::noncopyable, public std::enable_shared_from_this<SceneActor>
	{
	private:
		friend SceneSystem;

	// ----------------------------------------------------
	// Main

	private:
		SceneActor(World const& world);

	public:
		~SceneActor();

	private:
		World const& mWorld;

	// ----------------------------------------------------
	// Hierarchy

	public:
		std::shared_ptr<SceneActor> Parent() const;

		SceneActor& Parent(std::shared_ptr<SceneActor> const& desiredParent);

		std::shared_ptr<SceneActor> GetChild(std::size_t const index) const;

		std::size_t NumChild() const;

	private:
		std::weak_ptr<SceneActor> mParent{};

		std::vector<std::weak_ptr<SceneActor>> mChildren{};

	// ----------------------------------------------------
	// Components

	public:
		template <typename T>
		T& CreateComponent();

	private:
		std::vector<std::shared_ptr<SceneComponent>> mComponents{};
	};
}

// --------------------------------------------------------

template <typename T>
T& aiva::layer2::SceneActor::CreateComponent()
{
	return *mComponents.emplace_back(new T{ weak_from_this() });
}
