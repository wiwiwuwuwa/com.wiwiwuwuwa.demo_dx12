#pragma once
#include <pch.h>

namespace aiva::layer2
{
	struct SceneComponent;
	struct World;
}

namespace aiva::layer2
{
	struct SceneActor final : private boost::noncopyable, public std::enable_shared_from_this<SceneActor>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<SceneActor> Create(Args&&... args);

	public:
		~SceneActor();

	private:
		SceneActor(std::weak_ptr<aiva::layer2::World> const& world, tiny_utf8::string const& name);

	private:
		std::weak_ptr<aiva::layer2::World> mWorld{};

		tiny_utf8::string mName{};

	// ----------------------------------------------------
	// Hierarchy

	public:
		std::shared_ptr<SceneActor> Parent() const;

		SceneActor& Parent(std::shared_ptr<SceneActor> const& desiredParent);

		std::shared_ptr<SceneActor> GetChild(std::size_t const index) const;

		std::size_t NumChild() const;

	private:
		std::weak_ptr<SceneActor> mParent{};

		std::vector<std::shared_ptr<SceneActor>> mChildren{};

	// ----------------------------------------------------
	// Components

	public:
		template <typename T>
		std::weak_ptr<T> CreateComponent();

	private:
		std::vector<std::shared_ptr<aiva::layer2::SceneComponent>> mComponents{};
	};
}

// --------------------------------------------------------

template <typename T>
std::weak_ptr<T> aiva::layer2::SceneActor::CreateComponent()
{
	return mComponents.emplace_back(new T{ weak_from_this() });
}

template <typename... Args>
std::shared_ptr<aiva::layer2::SceneActor> aiva::layer2::SceneActor::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer2::SceneActor>(new aiva::layer2::SceneActor{ std::forward<Args>(args)... });
}
