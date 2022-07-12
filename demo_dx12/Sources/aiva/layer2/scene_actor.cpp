#include <pch.h>
#include <aiva/layer2/scene_actor.h>

aiva::layer2::SceneActor::SceneActor(std::weak_ptr<aiva::layer2::World> const& world, tiny_utf8::string const& name) : mWorld{ world }, mName{ name }
{

}

aiva::layer2::SceneActor::~SceneActor()
{

}

std::shared_ptr<aiva::layer2::SceneActor> aiva::layer2::SceneActor::Parent() const
{
	return mParent.lock();
}

aiva::layer2::SceneActor& aiva::layer2::SceneActor::Parent(std::shared_ptr<aiva::layer2::SceneActor> const& desiredParent)
{
	std::shared_ptr<aiva::layer2::SceneActor> const previousParent = mParent.lock();

	if (previousParent)
	{
		std::remove(previousParent->mChildren.begin(), previousParent->mChildren.end(), shared_from_this());
	}

	if (desiredParent)
	{
		desiredParent->mChildren.emplace_back(shared_from_this());
	}

	mParent = desiredParent;

	return *this;
}

std::shared_ptr<aiva::layer2::SceneActor> aiva::layer2::SceneActor::GetChild(std::size_t const index) const
{
	return mChildren.at(index);
}

std::size_t aiva::layer2::SceneActor::NumChild() const
{
	return mChildren.size();
}
