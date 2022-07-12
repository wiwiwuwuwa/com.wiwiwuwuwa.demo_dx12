#pragma once
#include <pch.h>

namespace aiva::layer2
{
	struct SceneComponent;
	struct SceneSystem;
}

namespace aiva::layer2
{
	struct SceneActor final : private boost::noncopyable, public std::enable_shared_from_this<SceneActor>
	{
	private:
		friend SceneSystem;

	// ----------------------------------------------------
	// Main

	public:
		~SceneActor();

	private:
		SceneActor(std::weak_ptr<aiva::layer2::SceneSystem> const& scene);

	private:
		std::weak_ptr<aiva::layer2::SceneSystem> mScene{};

	// ----------------------------------------------------
	// Hierarchy

	public:
		std::shared_ptr<aiva::layer2::SceneActor> Parent() const;

		aiva::layer2::SceneActor& Parent(std::shared_ptr<aiva::layer2::SceneActor> const& desiredParent);

		std::shared_ptr<aiva::layer2::SceneActor> GetChild(std::size_t const index) const;

		std::size_t NumChild() const;

	private:
		std::weak_ptr<aiva::layer2::SceneActor> mParent{};

		std::vector<std::weak_ptr<aiva::layer2::SceneActor>> mChildren{};

	// ----------------------------------------------------
	// Components

	public:
		template <typename T>
		T& CreateComponent();

	private:
		std::vector<std::shared_ptr<aiva::layer2::SceneComponent>> mComponents{};
	};
}

// --------------------------------------------------------

template <typename T>
T& aiva::layer2::SceneActor::CreateComponent()
{
	return *mComponents.emplace_back(new T{ weak_from_this() });
}
