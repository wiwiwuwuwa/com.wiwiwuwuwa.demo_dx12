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
	// Name

	public:
		std::string const& Name() const;

		SceneActor& Name(std::string const& name);

	private:
		std::string mName{};

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
	// Transformations

	// --------------------------------
	// Main

	public:
		glm::vec3 const& LocalPosition() const;

		SceneActor& LocalPosition(glm::vec3 const& position);

	private:
		glm::vec3 mLocalPosition = glm::zero<glm::vec3>();

	public:
		glm::quat const& LocalRotation() const;

		SceneActor& LocalRotation(glm::quat const& rotation);

	private:
		glm::quat mLocalRotation = glm::identity<glm::quat>();

	public:
		glm::vec3 const& LocalScale() const;

		SceneActor& LocalScale(glm::vec3 const& scale);

	private:
		glm::vec3 mLocalScale = glm::one<glm::vec3>();

	// --------------------------------
	// Extensions

	public:
		glm::mat4 LocalTransform() const;

		glm::mat4 WorldTransform() const;

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
