#include <pch.h>
#include <aiva/layer2/scene_actor.h>

#include <aiva/layer2/scene_system.h>
#include <aiva/layer2/world.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer2
{
	using namespace aiva::layer1;
	using namespace aiva::utils;

	SceneActor::SceneActor(aiva::layer2::World const& world) : AObject{}, mWorld{ world }
	{

	}

	aiva::layer2::World const& SceneActor::World() const
	{
		return mWorld;
	}

	SceneActorTypeShared SceneActor::SharedFromThis()
	{
		auto const basicShared = shared_from_this();
		if (!basicShared) return {};

		auto const specificShared = std::dynamic_pointer_cast<SceneActorType>(basicShared);
		Asserts::CheckBool(specificShared, "Specific shared is not valid");

		return specificShared;
	}

	SceneActorTypeWeak SceneActor::WeakFromThis()
	{
		return SharedFromThis();
	}

	SceneActor::~SceneActor()
	{
		auto const parent = mParent.lock();

		if (parent)
		{
			std::remove_if(parent->mChildren.begin(), parent->mChildren.end(), [this](auto const& child) { return child.lock() == SharedFromThis(); });
		}
	}

	std::string const& SceneActor::Name() const
	{
		return mName;
	}

	SceneActor& SceneActor::Name(std::string const& name)
	{
		mName = name;
		return *this;
	}

	SceneActorTypeShared SceneActor::Parent() const
	{
		return mParent.lock();
	}

	SceneActor& SceneActor::Parent(SceneActorTypeShared const& desiredParent)
	{
		auto const previousParent = mParent.lock();
		mParent = desiredParent;

		if (previousParent)
		{
			std::remove_if(previousParent->mChildren.begin(), previousParent->mChildren.end(), [this](auto const& child) { return child.lock() == SharedFromThis(); });
		}

		if (desiredParent)
		{
			desiredParent->mChildren.emplace_back(SharedFromThis());
		}

		return *this;
	}

	SceneActorTypeShared SceneActor::GetChild(std::size_t const index) const
	{
		return mChildren.at(index).lock();
	}

	std::size_t SceneActor::NumChild() const
	{
		return mChildren.size();
	}

	glm::vec3 const& SceneActor::LocalPosition() const
	{
		return mLocalPosition;
	}

	SceneActor& SceneActor::LocalPosition(glm::vec3 const& position)
	{
		mLocalPosition = position;
		return *this;
	}

	glm::quat const& SceneActor::LocalRotation() const
	{
		return mLocalRotation;
	}

	SceneActor& SceneActor::LocalRotation(glm::quat const& rotation)
	{
		mLocalRotation = rotation;
		return *this;
	}

	glm::vec3 const& SceneActor::LocalScale() const
	{
		return mLocalScale;
	}

	SceneActor& SceneActor::LocalScale(glm::vec3 const& scale)
	{
		mLocalScale = scale;
		return *this;
	}

	glm::mat4 SceneActor::LocalTransform() const
	{
		auto const translation = glm::translate(glm::identity<glm::mat4>(), LocalPosition());
		auto const rotation = glm::mat4_cast(LocalRotation());
		auto const scale = glm::scale(glm::identity<glm::mat4>(), LocalScale());

		return translation * rotation * scale;
	}

	glm::mat4 SceneActor::WorldTransform() const
	{
		if (Parent())
		{
			return Parent()->WorldTransform() * LocalTransform();
		}
		else
		{
			return LocalTransform();
		}
	}

	glm::mat4 SceneActor::WorldView() const
	{
		auto const transform = WorldTransform();

		auto const origin = glm::vec3(transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		auto const forward = glm::vec3(transform * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
		auto const upwards = glm::vec3(transform * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

		return glm::lookAt(origin, origin + forward, upwards);
	}
}