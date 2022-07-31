#include <pch.h>
#include <aiva/layer2/scene_actor.h>

#include <aiva/layer2/scene_system.h>
#include <aiva/utils/asserts.h>

aiva::layer2::SceneActor::SceneActor(World const& world) : mWorld{ world }
{

}

aiva::layer2::SceneActor::~SceneActor()
{
	auto const parent = mParent.lock();

	if (parent)
	{
		std::remove_if(parent->mChildren.begin(), parent->mChildren.end(), [this](auto const& child) { return child.lock() == shared_from_this(); });
	}
}

std::shared_ptr<aiva::layer2::SceneActor> aiva::layer2::SceneActor::Parent() const
{
	return mParent.lock();
}

aiva::layer2::SceneActor& aiva::layer2::SceneActor::Parent(std::shared_ptr<SceneActor> const& desiredParent)
{
	auto const previousParent = mParent.lock();
	mParent = desiredParent;

	if (previousParent)
	{
		std::remove_if(previousParent->mChildren.begin(), previousParent->mChildren.end(), [this](auto const& child) { return child.lock() == shared_from_this(); });
	}

	if (desiredParent)
	{
		desiredParent->mChildren.emplace_back(weak_from_this());
	}

	return *this;
}

std::shared_ptr<aiva::layer2::SceneActor> aiva::layer2::SceneActor::GetChild(std::size_t const index) const
{
	return mChildren.at(index).lock();
}

std::size_t aiva::layer2::SceneActor::NumChild() const
{
	return mChildren.size();
}

glm::vec3 const& aiva::layer2::SceneActor::LocalPosition() const
{
	return mLocalPosition;
}

aiva::layer2::SceneActor& aiva::layer2::SceneActor::LocalPosition(glm::vec3 const& position)
{
	mLocalPosition = position;
	return *this;
}

glm::quat const& aiva::layer2::SceneActor::LocalRotation() const
{
	return mLocalRotation;
}

aiva::layer2::SceneActor& aiva::layer2::SceneActor::LocalRotation(glm::quat const& rotation)
{
	mLocalRotation = rotation;
	return *this;
}

glm::vec3 const& aiva::layer2::SceneActor::LocalScale() const
{
	return mLocalScale;
}

aiva::layer2::SceneActor& aiva::layer2::SceneActor::LocalScale(glm::vec3 const& scale)
{
	mLocalScale = scale;
	return *this;
}

glm::mat4 aiva::layer2::SceneActor::LocalTransform() const
{
	auto trs = glm::identity<glm::mat4>();
	trs = glm::translate(trs, LocalPosition());
	trs = trs * glm::mat4_cast(LocalRotation());
	trs = glm::scale(trs, LocalScale());

	return trs;
}

glm::mat4 aiva::layer2::SceneActor::WorldTransform() const
{
	if (Parent())
	{
		return Parent()->LocalTransform() * LocalTransform();
	}
	else
	{
		return LocalTransform();
	}
}
