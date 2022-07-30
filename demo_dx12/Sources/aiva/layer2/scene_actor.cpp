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
