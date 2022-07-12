#pragma once
#include <pch.h>

namespace aiva::layer2
{
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
		SceneActor(aiva::layer2::World const& world, tiny_utf8::string const& name);

	private:
		aiva::layer2::World const& mWorld;

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
	};
}

// --------------------------------------------------------

template <typename... Args>
std::shared_ptr<aiva::layer2::SceneActor> aiva::layer2::SceneActor::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer2::SceneActor>(new aiva::layer2::SceneActor{ std::forward<Args>(args)... });
}
