#pragma once
#include <pch.h>

#include <aiva/layer2/scene_actor_fwd.h>
#include <aiva/layer2/scene_component_fwd.h>
#include <aiva/layer2/scene_system_fwd.h>
#include <aiva/utils/a_object.h>

namespace aiva::layer2
{
	struct World;
}

namespace aiva::layer2
{
	struct SceneActor final : public aiva::utils::AObject
	{
	// ----------------------------------------------------
	// Main

	private:
		friend SceneSystemType;

	protected:
		SceneActor(World const& world);

	public:
		~SceneActor();

	public:
		aiva::layer2::World const& World() const;

	private:
		aiva::layer2::World const& mWorld;

	// ----------------------------------------------------
	// Pointers

	public:
		SceneActorTypeShared SharedFromThis();

		SceneActorTypeWeak WeakFromThis();

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
		SceneActorTypeShared Parent() const;

		SceneActor& Parent(SceneActorTypeShared const& desiredParent);

		SceneActorTypeShared GetChild(std::size_t const index) const;

		std::size_t NumChild() const;

	private:
		SceneActorTypeWeak mParent{};

		std::vector<SceneActorTypeWeak> mChildren{};

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

		glm::mat4 WorldView() const;

	// ----------------------------------------------------
	// Components

	public:
		template <typename T>
		std::shared_ptr<T> CreateComponent();

	private:
		std::vector<std::shared_ptr<SceneComponent>> mComponents{};
	};
}

// --------------------------------------------------------

template <typename T>
std::shared_ptr<T> aiva::layer2::SceneActor::CreateComponent()
{
	auto const component = std::shared_ptr<T>(new T{ *this });
	mComponents.emplace_back(component);

	return component;
}
